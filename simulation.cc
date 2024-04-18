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

#define MAX_BYTES 50

using namespace ns3;

NodeContainer sender, destination, router;



int main(int argc, char *argv[])
{

    //creating topology
    sender.Create(2);
    destination.Create(2);
    router.Create(2);

    // Create point-to-point helper PointToPointHelper
    PointToPointHelper pointToPoint;

    //Install Internet stacks on each node
    InternetStackHelper internet;
    Ipv4AddressHelper address;
    internet.Install(sender);
    internet.Install(destination);
    internet.Install(router);


    // Create subnets
    NodeContainer link1 = NodeContainer(sender.Get(0), router.Get(0));
    NodeContainer link2 = NodeContainer(sender.Get(1), router.Get(0));
    NodeContainer link3 = NodeContainer(router.Get(0), router.Get(1));
    NodeContainer link4 = NodeContainer(router.Get(1), destination.Get(0));
    NodeContainer link5 = NodeContainer(router.Get(1), destination.Get(1));

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

    BulkSendHelper source ("ns3::TcpSocketFactory",InetSocketAddress(i4.GetAddress(1), 9));
    // Set the amount of data to send in bytes.  Zero is unlimited.
    source.SetAttribute("MaxBytes", UintegerValue(MAX_BYTES * 1024 * 1024));
    ApplicationContainer sourceApps = source.Install(sender.Get(0));
    sourceApps.Start(Seconds(0.0));
    sourceApps.Stop(Seconds(10.0));
    
    PacketSinkHelper sink ("ns3::TcpSocketFactory",InetSocketAddress(Ipv4Address::GetAny(), 9));
    ApplicationContainer sinkApps = sink.Install (destination.Get(0));
    sinkApps.Start(Seconds(0.0));
    sinkApps.Stop(Seconds(10.0));

    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    Simulator::Run();
    Simulator::Destroy();

    // Calculate the average throughput in bits per secondand flow completion time
    double totalBytes = 0;
    double totalFlowTime = 0;
    double totalFlows = 0;
    double throughput = 0;
    double flowTime = 0;

    flowMonitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowHelper.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i)
    {
        totalBytes += i->second.txBytes;
        totalFlowTime += i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds();
        totalFlows++;
    }

    throughput = (totalBytes * 8) / (totalFlowTime);
    flowTime = totalFlowTime / totalFlows;

    std::cout << "Average throughput: " << throughput << " bps" << std::endl;
    std::cout << "Average flow completion time: " << flowTime << " s" << std::endl;

    return 0;
}