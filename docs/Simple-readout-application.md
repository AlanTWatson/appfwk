## Simple construction
The simplest draft of a readout application can be

![simple_readout](https://user-images.githubusercontent.com/31312964/79783161-24618700-8338-11ea-819e-a0fb55829e67.png)

### DAQ Modules
* **Hardware specific Module** is the module that talk to the hardware and retrieves the data. Potentially is also the module that controls the hardware, but for this specific application is not necessary. 
It is supposed to have a single bugger which is a QueueSink. It is supposed to use at least one thread to do the push operation on the Queue plus all the threads necessary to perform the required operations.
* **Packager** is an intermediate module that is supposed to group the data coming from the hardware specific module in a way which is consumable from the downstream part of the DAQ. Depending on the output of the Hardware specific module this module can be general or also hardware specific. Ideally it only uses one thread, but depending on the type of packaging we need a more elaborate internal structure might be necessary, but it is hard to imagine anything too complex until we have a decision on the data format we want. 
* **Sender** to storage is a generic module that takes data in a standard format and sent it to the storage to be digested by the rest of the DAQ or offline, depending on configuration. It is expected that the sender has a socket that connects to other nodes in the network. This Module is expected to use 1 thread or 2 at most if particular protocols are expected via the socket communication. 

### Queues
This configuration requires only one type of queue, a *SPSC queue*, both of them expected to be thread safe. Yet the two will be different. The first (from hardware specific to packager) is a queue of small chunks of memory. The second is a queue of bigger memory that are following a standard format. 


## Advanced construction
The readout application can be enhanced a bit to make it generate human readable information in something similar to this graph:

![advanced_readout](https://user-images.githubusercontent.com/31312964/81170765-214ed380-8f93-11ea-88b1-d40efe58cf8c.png)

At any point, the chain of the simple graph can interrupted by a fan-out module that can simply copy the information and stream it to CCM for debug or other. 

### DAQ Modules
* a **Fan-out** module is required to do this operation. The module is supposed to take whatever is the input and copy it in each output queue. Ideally this kind of Modules will be provided by the framework and should be on the user side except if a particular functionality is required. The module is expected to use only one thread.
* A dedicated module is necessary to either create  metrics or dump the information on screen. The **metric publisher** is probably hardware related as it needs to decode the data from the hardware in a specific way. Standard metrics are supposed to be generated by the framework anyway  so for those a dedicated module is not necessary. A **Dumper** module instead can be fully generic if the only thing that does is printing the binary content of the information propagated by the application. One thread should be enough for the operation. 

### Queues 
The whole graph can be build using only *SPSC queues* as per the simply construction. Anyway the version that dumps information on screen can have a circular buffer instead of a queue. This can be used to limit the amount of output on screen if it becomes not manageable. 

## Producer/Consumer Test Application

![image](https://user-images.githubusercontent.com/61473357/85172069-ed272d80-b235-11ea-8d3e-d99834b16102.png)

The Application Framework v1 supplies [`producer_consumer_dynamic_test.json`](https://github.com/DUNE-DAQ/appfwk/blob/develop/test/producer_consumer_dynamic_test.json), which is a mock-up of the above application. FakeDataProducer creates vectors of integers, which are given to a FanOutDAQModule, which distributes them to two FakeDataConsumers.