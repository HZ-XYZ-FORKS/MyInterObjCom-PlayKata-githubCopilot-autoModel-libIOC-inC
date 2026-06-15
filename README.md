# MyInterObjComBase 【我的对象间通讯基地】

- Created by EnigmaWU on 2026-05-17
  - based initially on [MyIOC_inTDD_withGHC](https://github.com/EnigmaWU/MyIOC_inTDD_withGHC)
  - follows experiences from [MyGenCodeDescBase](https://github.com/EnigmaWU/MyGenCodeDescBase)
- Used to `PlayKata` with:
  - `CaTDD`: [Comment-alive Test-Driven Development](https://github.com/EnigmaWU/MyCaTDD)
  - `Skills`: [MyCodeAgentSkills](https://github.com/EnigmaWU/MyCodeAgentSkills)

## >>>>>>WHAT WE HAVE<<<<<<<

- Imported IOC interface headers are under `Include/IOC` (source: [MyIOC_inTDD_withGHC/Include](https://github.com/EnigmaWU/MyIOC_inTDD_withGHC/tree/main/Include)).

- We defined IOC HAS:
  - `Service` and `Link` as the two main concepts of IOC.
  - While `Service` MAY online/offline, and `Link` is established between `Service` and its `Client`, then the `Link` MAY send/recvData, post/pull/subEvent, exec/waitCommand, until the `Link` is closed by either side.

- Detail: [Usage Scenarios](Doc/IOC_UsageScenarios.md) 

## >>>>>>WHY DATA/EVENT/COMMAND<<<<<<

- **CMD** (Synchronous Request-Response): ObjX always gets final result or failure reason.
  - SYNC + NODROP = certainty (e.g., RPCs, configuration commands, critical operations)
  
- **EVT** (Asynchronous Fire-and-Forget): ObjX sends notification; ObjY may or may not receive.
  - ASYNC + MAYDROP = efficiency (e.g., status updates, alerts, one-to-many broadcasts)
  
- **DAT** (Asynchronous Streaming): ObjX sends data chunks; flow control ensures delivery.
  - ASYNC + NODROP + Stream = reliability for sequential integrity (e.g., file transfer, log streaming, DB replication)

> **Design Insight**: Three roles separate concerns.
>
> - CMD for control (certainty needed).
> - EVT for signals (throughput preferred).
> - DAT for transfer (order + completeness required).

## >>>>>>WHAT WE WANT<<<<<<<

- Design and implement and test IOC with [CaTDD](https://github.com/EnigmaWU/MyCaTDD) in the form of such as:
  - libIOC.a/.so in C or C++
  - OpenAPI in Typescript or Python
  - and so on...