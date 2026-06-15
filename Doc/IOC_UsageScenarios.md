# IOC Usage Scenarios

- We can use `IOC` to establish a `Link` then both service and client can be one `Usage`, such as:
  - Link@Service-asDataSender <-> Link@Client-asDataReceiver
  - Link@Service-asDataReceiver <-> Link@Client-asDataSender
  - Link@Service-asEventProducer <-> Link@Client-asEventConsumer
  - Link@Service-asEventConsumer <-> Link@Client-asEventProducer
  - Link@Service-asCommandExecutor <-> Link@Client-asCommandInitiator
  - Link@Service-asCommandInitiator <-> Link@Client-asCommandExecutor

## Established Link

## Data Send/Receive

## Event Produce/Consume

## Command Initiate/Execute
