#include <iostream>
#include <fstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/bridge-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/traffic-control-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Project");
// Trace function (sink)
// Trace function (sink)

int
main (int argc, char *argv[])
{
  RngSeedManager::SetSeed(1);
  RngSeedManager::SetRun(1);
  //
  // Allow the user to override any of the defaults and the above Bind() at
  // run-time, via command-line arguments
  //
  
  #if 1
  LogComponentEnable ("Project", LOG_LEVEL_INFO);
  LogComponentEnable("PacketSink", LOG_LEVEL_INFO);
  #endif
  
  
  
  
  CommandLine cmd;
  cmd.Parse (argc, argv);

//  
//       Current topology:
//  
//	    N0         N5
//  	N1  |          |  N6
//  	  \ |          | /
//    N2-- S1----------S2 --N7
//        / |          | \
//      N3  |          |  N8
//          N4         N9
//  


  //Create a node container which keeps track of a set of node pointers
  NS_LOG_INFO ("Create nodes.");
  NodeContainer terminals;
  terminals.Create (10); 

  //Set TCP congestion control algorithm
  Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpLinuxReno"));
  
  //Switches
  NodeContainer csmaSwitch;
  csmaSwitch.Create (2);
  
  NS_LOG_INFO ("Build Topology");
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate("100Mbps")));                               
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  

  // Create the csma links, from each terminal to the switch
  NetDeviceContainer terminalDevices;
  NetDeviceContainer switch1Devices;
  NetDeviceContainer switch2Devices;
	
  //Link terminal -> switch (switch 1)
  for (int i = 0; i < 5; i++)
    {
      NetDeviceContainer link = csma.Install (NodeContainer (terminals.Get (i), csmaSwitch.Get(0)));
      terminalDevices.Add (link.Get (0));
      switch1Devices.Add (link.Get (1));
    }
  //Link terminal -> switch (switch 2)
  for (int i = 5; i < 10; i++)
    {
      NetDeviceContainer link = csma.Install (NodeContainer (terminals.Get (i), csmaSwitch.Get(1)));
      terminalDevices.Add (link.Get (0));
      switch2Devices.Add (link.Get (1));
    }


  //connecting the two switches
  NetDeviceContainer link = csma.Install (NodeContainer (csmaSwitch.Get (0), csmaSwitch.Get(1)));
  switch1Devices.Add (link.Get (0));
  switch2Devices.Add (link.Get (1));

  // Create the bridge netdevice, which will do the packet switching
  Ptr<Node> switchNode = csmaSwitch.Get (0);
  BridgeHelper bridge;
  bridge.Install (switchNode, switch1Devices);
  switchNode = csmaSwitch.Get(1);
  bridge.Install (switchNode, switch2Devices);

  // Add internet stack to the terminals
  InternetStackHelper internet;
  internet.Install (terminals);

 
  // We've got the "hardware" in place.  Now we need to add IP addresses.
  //
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  ipv4.Assign (terminalDevices);

  //----
  uint16_t port = 9;   // Discard port (RFC 863)


  //Make destinations for onoff communication
  OnOffHelper onoff0 ("ns3::TcpSocketFactory", 
                     Address (InetSocketAddress (Ipv4Address ("10.1.1.1"), port))); //Create destination for N0
  onoff0.SetConstantRate (DataRate ("80kb/s"));
  
  OnOffHelper onoff1 ("ns3::TcpSocketFactory", 
                     Address (InetSocketAddress (Ipv4Address ("10.1.1.2"), port))); //Create destination for N1
  onoff1.SetConstantRate (DataRate ("80kb/s"));
  
  OnOffHelper onoff2 ("ns3::TcpSocketFactory", 
                     Address (InetSocketAddress (Ipv4Address ("10.1.1.3"), port))); //Create destination for N2
  onoff2.SetConstantRate (DataRate ("80kb/s"));
  
  OnOffHelper onoff3 ("ns3::TcpSocketFactory", 
                     Address (InetSocketAddress (Ipv4Address ("10.1.1.4"), port))); //Create destination for N3
  onoff3.SetConstantRate (DataRate ("80kb/s"));
  
  OnOffHelper onoff4 ("ns3::TcpSocketFactory", 
                     Address (InetSocketAddress (Ipv4Address ("10.1.1.5"), port))); //Create destination for N4
  onoff4.SetConstantRate (DataRate ("80kb/s"));
  
  OnOffHelper onoff5 ("ns3::TcpSocketFactory", 
                     Address (InetSocketAddress (Ipv4Address ("10.1.1.6"), port))); //Create destination for N5
  onoff5.SetConstantRate (DataRate ("80kb/s"));
  
  OnOffHelper onoff6 ("ns3::TcpSocketFactory", 
                     Address (InetSocketAddress (Ipv4Address ("10.1.1.7"), port))); //Create destination for N6
  onoff6.SetConstantRate (DataRate ("80kb/s"));
  
  OnOffHelper onoff7 ("ns3::TcpSocketFactory", 
                     Address (InetSocketAddress (Ipv4Address ("10.1.1.8"), port))); //Create destination for N7
  onoff7.SetConstantRate (DataRate ("80kb/s"));
  
  OnOffHelper onoff8 ("ns3::TcpSocketFactory", 
                     Address (InetSocketAddress (Ipv4Address ("10.1.1.9"), port))); //Create destination for N8
  onoff8.SetConstantRate (DataRate ("80kb/s"));
  
  OnOffHelper onoff9 ("ns3::TcpSocketFactory", 
                     Address (InetSocketAddress (Ipv4Address ("10.1.1.10"), port))); //Create destination for N9
  onoff9.SetConstantRate (DataRate ("80kb/s"));
  

  // Create packetsink helper
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
                         
  // Application sends from N9 to N0
  ApplicationContainer app1 = onoff0.Install (terminals.Get (9));
  
  app1.Start (Seconds (1.0));
  app1.Stop (Seconds (15.0)); 

  app1 = sink.Install (terminals.Get (0));
  
  
  // Application sends from N7 to N3
  ApplicationContainer app2 = onoff3.Install (terminals.Get (7));
  
  app2.Start (Seconds (1.0));
  app2.Stop (Seconds (15.0)); 

  app2 = sink.Install (terminals.Get (3));

  

  
  NS_LOG_INFO ("Configure Tracing.");

  
  AsciiTraceHelper ascii;
  csma.EnableAsciiAll ("Project");
  csma.EnablePcapAll ("Project", false);
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();

  NS_LOG_INFO ("Done.");




}
