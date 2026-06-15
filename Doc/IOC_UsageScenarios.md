# IOC Usage Scenarios

> Notes
> - This document is scenario-oriented. It focuses on IOC usage semantics and interaction roles.
> - Sequence diagrams use current C APIs as semantic anchors, not as language constraints.
> - Equivalent wrappers/bindings in other languages are valid if they preserve the same IOC semantics.

- We can use `IOC` to establish a `Link` then both service and client can be one `Usage`, such as:
  - Link@Service-asDataSender <-> Link@Client-asDataReceiver
  - Link@Service-asDataReceiver <-> Link@Client-asDataSender
  - Link@Service-asEventProducer <-> Link@Client-asEventConsumer
  - Link@Service-asEventConsumer <-> Link@Client-asEventProducer
  - Link@Service-asCommandExecutor <-> Link@Client-asCommandInitiator
  - Link@Service-asCommandInitiator <-> Link@Client-asCommandExecutor

## Established Link

**Acceptance Criteria:**
- Given: Service App calls IOC_onlineService() with valid SrvID
- When: Client App calls IOC_connectService() with matching Usage
- Then: IOC establishes bidirectional LinkID pair (AcptLinkID <-> ConnLinkID)
- And: Both Service and Client can use their respective LinkID for messaging
- And: Service receives OnAutoAccepted_F callback (if configured) with AcptLinkID

```mermaid
sequenceDiagram
  autonumber
  participant SApp as Service App
  participant IOCS as IOC(Service Side)
  participant CApp as Client App
  participant IOCC as IOC(Client Side)

  SApp->>IOCS: IOC_onlineService(&SrvID, &SrvArgs)
  IOCS-->>SApp: IOC_RESULT_SUCCESS + SrvID
  Note over SApp,IOCS: SrvArgs.UsageCapabilites must include complementary usage

  CApp->>IOCC: IOC_connectService(&ConnLinkID, &ConnArgs, pOption)
  IOCC->>IOCS: Connect by ConnArgs.SrvURI + ConnArgs.Usage
  Note over CApp,IOCC: ConnArgs.Usage is one of DAT/EVT/CMD roles

  alt Service uses IOC_SRVFLAG_AUTO_ACCEPT
    IOCS-->>IOCS: Auto create AcptLinkID in background
    IOCS-->>IOCC: Connection accepted
    IOCC-->>CApp: IOC_RESULT_SUCCESS + ConnLinkID
    opt OnAutoAccepted_F configured
      IOCS-->>SApp: OnAutoAccepted_F(SrvID, AcptLinkID, pPriv)
    end
  else Manual accept mode
    IOCS-->>SApp: Pending client available
    SApp->>IOCS: IOC_acceptClient(SrvID, &AcptLinkID, pOption)
    IOCS-->>SApp: IOC_RESULT_SUCCESS + AcptLinkID
    IOCS-->>IOCC: Connection accepted
    IOCC-->>CApp: IOC_RESULT_SUCCESS + ConnLinkID
  end

  Note over SApp,CApp: Established pair in default P2P: AcptLinkID <-> ConnLinkID
  Note over SApp,IOCS: Service shutdown: IOC_offlineService(SrvID)
```

## Data Send/Receive

### US-1: Service as DataReceiver, Client as DataSender

**Acceptance Criteria:**
- Given: Link established with Service.AcptLinkID and Client.ConnLinkID
- And: CbRecvDat_F callback configured on Service side
- When: Client calls IOC_sendDAT(ConnLinkID, &DatDesc) repeatedly
- Then: Service receives each DatDesc via CbRecvDat_F(AcptLinkID, &DatDesc)
- And: Order and completeness of data chunks guaranteed (NODROP + STREAM)
- And: Optional IOC_flushDAT() ensures buffered chunks delivered

Precondition: Link is already established as AcptLinkID <-> ConnLinkID.

```mermaid
sequenceDiagram
  autonumber
  participant SApp as Service App
  participant IOCS as IOC(Service Side)
  participant CApp as Client App
  participant IOCC as IOC(Client Side)

  Note over SApp,IOCS: Receiver side has CbRecvDat_F configured on AcptLinkID
  Note over CApp,IOCC: Sender uses ConnLinkID for stream sending

  loop Stream chunks
    CApp->>IOCC: IOC_sendDAT(ConnLinkID, &DatDesc, pOption)
    IOCC->>IOCS: Deliver DAT chunk
    IOCS-->>SApp: ServiceRecvCb(AcptLinkID, &DatDesc, pCbPriv)
  end

  opt Force buffered data out
    CApp->>IOCC: IOC_flushDAT(ConnLinkID, pOption)
    IOCC-->>CApp: IOC_RESULT_SUCCESS
  end
```

### US-2: Service as DataSender, Client as DataReceiver

**Acceptance Criteria:**
- Given: Link established with Service.AcptLinkID and Client.ConnLinkID
- When: Service calls IOC_sendDAT(AcptLinkID, &DatDesc)
- Then: Client receives data either via:
  - Callback mode: CbRecvDat_F(ConnLinkID, &DatDesc), OR
  - Polling mode: IOC_recvDAT(ConnLinkID, &DatDesc) returns IOC_RESULT_SUCCESS
- And: All chunks delivered in order with no loss
- And: Service can flush with IOC_flushDAT(AcptLinkID) to force delivery

Precondition: Link is already established as AcptLinkID <-> ConnLinkID.

```mermaid
sequenceDiagram
  autonumber
  participant SApp as Service App
  participant IOCS as IOC(Service Side)
  participant CApp as Client App
  participant IOCC as IOC(Client Side)

  Note over SApp,IOCS: Sender uses AcptLinkID for stream sending
  Note over CApp,IOCC: Receiver uses ConnLinkID via callback or polling

  alt Callback receive mode
    SApp->>IOCS: IOC_sendDAT(AcptLinkID, &DatDesc, pOption)
    IOCS->>IOCC: Deliver DAT chunk
    IOCC-->>CApp: ClientRecvCb(ConnLinkID, &DatDesc, pCbPriv)
  else Polling receive mode
    SApp->>IOCS: IOC_sendDAT(AcptLinkID, &DatDesc, pOption)
    IOCS->>IOCC: Queue DAT chunk for receiver
    CApp->>IOCC: IOC_recvDAT(ConnLinkID, &DatDesc, pOption)
    IOCC-->>CApp: IOC_RESULT_SUCCESS + payload
  end

  opt Service sender flush
    SApp->>IOCS: IOC_flushDAT(AcptLinkID, pOption)
    IOCS-->>SApp: IOC_RESULT_SUCCESS
  end
```

## Event Produce/Consume

### US-1: Service as EvtProducer, Client as EvtConsumer

**Acceptance Criteria:**
- Given: Link established with Service.AcptLinkID and Client.ConnLinkID
- When: Client subscribes via IOC_subEVT(ConnLinkID, &SubEvtArgs)
- And: Service posts event via IOC_postEVT(AcptLinkID, &EvtDesc)
- Then: Client receives event either via:
  - Callback mode: CbProcEvt_F(&EvtDesc) registered via subscription, OR
  - Polling mode: IOC_pullEVT(ConnLinkID, &EvtDesc) returns IOC_RESULT_SUCCESS
- And: May drop acceptable (ASYNC + MAYDROP semantics)
- And: Client can unsubscribe via IOC_unsubEVT() to stop receiving

Precondition: Link is already established as AcptLinkID <-> ConnLinkID.

```mermaid
sequenceDiagram
  autonumber
  participant SApp as Service App
  participant IOCS as IOC(Service Side)
  participant CApp as Client App
  participant IOCC as IOC(Client Side)

  alt Callback consumption mode
    Note over CApp,IOCC: Consumer subscribes with callback on ConnLinkID
    CApp->>IOCC: IOC_subEVT(ConnLinkID, &SubEvtArgs)
    IOCC-->>CApp: IOC_RESULT_SUCCESS
    
    loop Event production and delivery
      SApp->>IOCS: IOC_postEVT(AcptLinkID, &EvtDesc, pOption)
      IOCS->>IOCC: Deliver EVT
      IOCC-->>CApp: CbProcEvt_F(&EvtDesc, pCbPriv)
    end
  else Polling consumption mode
    Note over CApp,IOCC: Consumer polls events from ConnLinkID
    
    loop Event production and polling
      SApp->>IOCS: IOC_postEVT(AcptLinkID, &EvtDesc, pOption)
      IOCS->>IOCC: Queue EVT
      CApp->>IOCC: IOC_pullEVT(ConnLinkID, &EvtDesc, pOption)
      IOCC-->>CApp: IOC_RESULT_SUCCESS + EvtDesc
    end
  end

  opt Unsubscribe from event (callback mode only)
    CApp->>IOCC: IOC_unsubEVT(ConnLinkID, &UnsubEvtArgs)
    IOCC-->>CApp: IOC_RESULT_SUCCESS
  end
```

### US-2: Service as EvtConsumer, Client as EvtProducer

**Acceptance Criteria:**
- Given: Link established with Service.AcptLinkID and Client.ConnLinkID
- When: Service subscribes via IOC_subEVT(AcptLinkID, &SubEvtArgs)
- And: Client posts event via IOC_postEVT(ConnLinkID, &EvtDesc)
- Then: Service receives event either via:
  - Callback mode: CbProcEvt_F(&EvtDesc) registered via subscription, OR
  - Polling mode: IOC_pullEVT(AcptLinkID, &EvtDesc) returns IOC_RESULT_SUCCESS
- And: Delivery may be dropped (fire-and-forget pattern)
- And: Service can unsubscribe via IOC_unsubEVT() to stop receiving

Precondition: Link is already established as AcptLinkID <-> ConnLinkID.

```mermaid
sequenceDiagram
  autonumber
  participant SApp as Service App
  participant IOCS as IOC(Service Side)
  participant CApp as Client App
  participant IOCC as IOC(Client Side)

  alt Callback consumption mode
    Note over SApp,IOCS: Consumer subscribes with callback on AcptLinkID
    SApp->>IOCS: IOC_subEVT(AcptLinkID, &SubEvtArgs)
    IOCS-->>SApp: IOC_RESULT_SUCCESS
    
    loop Event production and delivery
      CApp->>IOCC: IOC_postEVT(ConnLinkID, &EvtDesc, pOption)
      IOCC->>IOCS: Deliver EVT
      IOCS-->>SApp: CbProcEvt_F(&EvtDesc, pCbPriv)
    end
  else Polling consumption mode
    Note over SApp,IOCS: Consumer polls events from AcptLinkID
    
    loop Event production and polling
      CApp->>IOCC: IOC_postEVT(ConnLinkID, &EvtDesc, pOption)
      IOCC->>IOCS: Queue EVT
      SApp->>IOCS: IOC_pullEVT(AcptLinkID, &EvtDesc, pOption)
      IOCS-->>SApp: IOC_RESULT_SUCCESS + EvtDesc
    end
  end

  opt Unsubscribe from event (callback mode only)
    SApp->>IOCS: IOC_unsubEVT(AcptLinkID, &UnsubEvtArgs)
    IOCS-->>SApp: IOC_RESULT_SUCCESS
  end
```

## Command Initiate/Execute

### US-1: Client as CmdInitiator, Service as CmdExecutor

**Acceptance Criteria:**
- Given: Link established with Service.AcptLinkID and Client.ConnLinkID
- And: CbExecCmd_F callback configured on Service side (callback mode)
- When: Client calls IOC_execCMD(ConnLinkID, &CmdDesc)
- Then: Service receives command via CbExecCmd_F(AcptLinkID, &CmdDesc)
- And: Service executes and sets result in CmdDesc
- And: Client always receives final result/status (SYNC + NODROP)
- And: Polling mode alternative: IOC_waitCMD() + IOC_ackCMD() for explicit control

Precondition: Link is already established as AcptLinkID <-> ConnLinkID.

```mermaid
sequenceDiagram
  autonumber
  participant SApp as Service App
  participant IOCS as IOC(Service Side)
  participant CApp as Client App
  participant IOCC as IOC(Client Side)

  Note over SApp,IOCS: Executor side has CbExecCmd_F configured on AcptLinkID
  Note over CApp,IOCC: Initiator uses ConnLinkID to issue commands

  alt Callback execution mode
    CApp->>IOCC: IOC_execCMD(ConnLinkID, &CmdDesc, pOption)
    IOCC->>IOCS: Send CMD request
    IOCS-->>SApp: CbExecCmd_F(AcptLinkID, &CmdDesc, pCbPriv)
    SApp-->>SApp: Execute and set result in CmdDesc
    IOCS-->>IOCC: Return result with CmdDesc
    IOCC-->>CApp: IOC_RESULT_SUCCESS + result in CmdDesc
  else Polling execution mode
    CApp->>IOCC: IOC_execCMD(ConnLinkID, &CmdDesc, pOption)
    IOCC->>IOCS: Send CMD request
    SApp->>IOCS: IOC_waitCMD(AcptLinkID, &CmdDesc, pOption)
    IOCS-->>SApp: IOC_RESULT_SUCCESS + CmdDesc
    SApp-->>SApp: Execute and fill result in CmdDesc
    SApp->>IOCS: IOC_ackCMD(AcptLinkID, &CmdDesc, pOption)
    IOCS->>IOCC: Return result
    IOCC-->>CApp: IOC_RESULT_SUCCESS + result
  end
```

### US-2: Service as CmdInitiator, Client as CmdExecutor

**Acceptance Criteria:**
- Given: Link established with Service.AcptLinkID and Client.ConnLinkID
- And: CbExecCmd_F callback configured on Client side (callback mode)
- When: Service calls IOC_execCMD(AcptLinkID, &CmdDesc)
- Then: Client receives command via CbExecCmd_F(ConnLinkID, &CmdDesc)
- And: Client executes and sets result in CmdDesc
- And: Service always receives final result/status (SYNC + NODROP)
- And: Polling mode alternative: IOC_waitCMD() + IOC_ackCMD() for explicit control

Precondition: Link is already established as AcptLinkID <-> ConnLinkID.

```mermaid
sequenceDiagram
  autonumber
  participant SApp as Service App
  participant IOCS as IOC(Service Side)
  participant CApp as Client App
  participant IOCC as IOC(Client Side)

  Note over CApp,IOCC: Executor side has CbExecCmd_F configured on ConnLinkID
  Note over SApp,IOCS: Initiator uses AcptLinkID to issue commands

  alt Callback execution mode
    SApp->>IOCS: IOC_execCMD(AcptLinkID, &CmdDesc, pOption)
    IOCS->>IOCC: Send CMD request
    IOCC-->>CApp: CbExecCmd_F(ConnLinkID, &CmdDesc, pCbPriv)
    CApp-->>CApp: Execute and set result in CmdDesc
    IOCC->>IOCS: Return result with CmdDesc
    IOCS-->>SApp: IOC_RESULT_SUCCESS + result in CmdDesc
  else Polling execution mode
    SApp->>IOCS: IOC_execCMD(AcptLinkID, &CmdDesc, pOption)
    IOCS->>IOCC: Send CMD request
    CApp->>IOCC: IOC_waitCMD(ConnLinkID, &CmdDesc, pOption)
    IOCC-->>CApp: IOC_RESULT_SUCCESS + CmdDesc
    CApp-->>CApp: Execute and fill result in CmdDesc
    CApp->>IOCC: IOC_ackCMD(ConnLinkID, &CmdDesc, pOption)
    IOCC->>IOCS: Return result
    IOCS-->>SApp: IOC_RESULT_SUCCESS + result
  end
```
