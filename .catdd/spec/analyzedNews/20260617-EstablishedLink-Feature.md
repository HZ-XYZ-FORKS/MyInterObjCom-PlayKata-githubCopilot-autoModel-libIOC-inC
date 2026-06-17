# Feature: Established Link

**Source:** Doc/IOC_UsageScenarios.md § Established Link

**Classification:** Feature (Core IOC Capability)

**Intent:** Enable Service and Client applications to establish a bidirectional communication link through IOC, with support for both auto-accept and manual-accept modes.

## Feature Summary

The IOC library must allow a Service App to come online and a Client App to connect, establishing a pair of linked communication channels (AcptLinkID on Service side, ConnLinkID on Client side) that enable subsequent data, event, and command messaging.

## Source Text

### Acceptance Criteria:
- Given: Service App calls IOC_onlineService() with valid SrvID
- When: Client App calls IOC_connectService() with matching Usage
- Then: IOC establishes bidirectional LinkID pair (AcptLinkID <-> ConnLinkID)
- And: Both Service and Client can use their respective LinkID for messaging
- And: Service receives OnAutoAccepted_F callback (if configured) with AcptLinkID

### Sequence Flow (Excerpt):

**Auto-Accept Mode:**
- Service comes online with IOC_onlineService()
- Client initiates connection with IOC_connectService()
- IOC automatically creates AcptLinkID on Service side
- IOC returns ConnLinkID to Client
- Service receives OnAutoAccepted_F callback (if configured)

**Manual Accept Mode:**
- Service comes online with IOC_onlineService()
- Client initiates connection with IOC_connectService()
- IOC notifies Service of pending client
- Service explicitly calls IOC_acceptClient() to create AcptLinkID
- IOC returns ConnLinkID to Client

**Lifecycle End:**
- Service shutdown via IOC_offlineService(SrvID)

## Open Questions

- Which acceptance mode (auto vs. manual) should be the default behavior?
- What happens to existing connections if a Service calls IOC_offlineService()?
- Are there any timing constraints on the connection establishment?
- Should connection failures be retryable from the Client side?

## Related IOC Usage Patterns

This feature is a prerequisite for:
- Data Send/Receive (DAT) scenarios
- Event Produce/Consume (EVT) scenarios
- Command Initiate/Execute (CMD) scenarios

All of these patterns depend on a successfully established Link pair.
