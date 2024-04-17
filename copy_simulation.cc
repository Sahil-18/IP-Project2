#include <fstream>
#include <string>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("IP_PA2");
NodeContainer source, destination, router
double flow_time = 60.0;
double start_time = 0.0;
int d1_port = 8331, d2_port = 8331;
uint maxBytes = 50 * 1024 * 1024; 

int main(int argc, char *argv[])
{

    //creating topology
    source.Create(2);
    destination.Create(2);
    router.Create(2);

    // Create point-to-point helper PointToPointHelper
    PointToPointHelper pointToPoint;

    //Install Internet stacks on each node
    InternetStackHelper internet;
    Ipv4AddressHelper address;
    internet.Install(source);
    internet.Install(destination);
    internet.Install(router);


    // Create subnets
    NodeContainer link1 = NodeContainer(source.Get(0), router.Get(0));
    NodeContainer link2 = NodeContainer(source.Get(1), router.Get(0));
    NodeContainer link3 = NodeContainer(router.Get(0), router.Get(1));
    NodeContainer link4 = NodeContainer(router.Get(1), destination.Get(1));
    NodeContainer link5 = NodeContainer(router.Get(1), destination.Get(0));

    pointToPoint.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("5ms"));

    //Assigning Ips
    address.SetBase("10.1.1.0", "255.255.255.0");
    NetDeviceContainer s1linkr1 = pointToPoint.Install(link1);
    Ipv4InterfaceContainer i1 = address.Assign(s1linkr1);


    address.SetBase("10.1.2.0", "255.255.255.0");
    NetDeviceContainer s2linkr1 = pointToPoint.Install(link2);
    Ipv4InterfaceContainer i2 = address.Assign(s2linkr1);


    address.SetBase("10.1.3.0", "255.255.255.0");
    NetDeviceContainer r1linkr2 = pointToPoint.Install(link3);
    Ipv4InterfaceContainer i3 = address.Assign(r1linkr2);


    address.SetBase("10.1.4.0", "255.255.255.0");
    NetDeviceContainer r2linkd1 = pointToPoint.Install(link5);
    Ipv4InterfaceContainer i4 = address.Assign(r2linkd1);

    address.SetBase("10.1.5.0", "255.255.255.0");
    NetDeviceContainer r2linkd2 = pointToPoint.Install(link4);
    Ipv4InterfaceContainer i5 = address.Assign(r2linkd2);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    std::vector<double> tputs1, tputs2, ftime1, ftime2;
//experiment 1

    for(int i=0; i<3; i++, start_time += flow_time)
    {
    Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpBic::GetTypeId()));
  
    

    BulkSendHelper sourceHelper("ns3::TcpSocketFactory", i1);
    sourceHelper.SetAttribute("MaxBytes", UintegerValue(maxBytes));
   
    PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), 8331));
    ApplicationContainer dest_container = packetSinkHelper.Install(destination.Get(0));
    ApplicationContainer source_container = sourceHelper.Install(source.Get(0));

    dest_container.Start(Seconds(start_time));
    dest_container.Stop(Seconds(start_time+flow_time));
   
    source_container.Start(Seconds(start_time));
    source_container.Stop(Seconds(start_time+flow_time));


    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();
    monitor->CheckForLostPackets();
    
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

    }
   
    Simulator::Run();
    Simulator::Destroy();
    for(int i=0; i<3; i++){
        FlowMonitor::FlowStats fs = stats[flow_id];
        time_taken = fs.timeLastRxPacket.GetSeconds()-fs.timeFirstTxPacket.GetSeconds();
        curr_throughput = fs.rxBytes*8.0/time_taken;
        cout<<curr_throughput;
      

        flow_id += 2;
    }
}