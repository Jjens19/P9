#include "tcp-linux-reno.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("TcpLinuxReno");
NS_OBJECT_ENSURE_REGISTERED(TcpLinuxReno);

TypeId
TcpLinuxReno::GetTypeId()
{
    static TypeId tid = TypeId("ns3::TcpLinuxReno")
                            .SetParent<TcpCongestionOps>()
                            .SetGroupName("Internet")
                            .AddConstructor<TcpLinuxReno>();
    return tid;
}

TcpLinuxReno::TcpLinuxReno()
    : TcpCongestionOps()
{
    NS_LOG_FUNCTION(this);
}

TcpLinuxReno::TcpLinuxReno(const TcpLinuxReno& sock)
    : TcpCongestionOps(sock)
{
    NS_LOG_FUNCTION(this);
}

TcpLinuxReno::~TcpLinuxReno()
{
}


uint32_t
TcpLinuxReno::SlowStart(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
    NS_LOG_FUNCTION(this << tcb << segmentsAcked);

    if (segmentsAcked >= 1)
    {
        uint32_t sndCwnd = tcb->m_cWnd;
        
        // TESTING
    //Open file
	std::ifstream inputFile("/home/ubuntu/ns-allinone-3.39/ns-3.39/scratch/update.txt");
	
	// Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return; // Return an error code
    }
    
    int value;
    inputFile >> value;

	// Check if the read operation was successful
    if (!inputFile) {
        std::cerr << "Error reading from the file." << std::endl;
        return; // Return an error code
    }
    
    // Close the file
    inputFile.close();
    
    if (value == 1) {
    	std::ofstream outputFile("/home/ubuntu/ns-allinone-3.39/ns-3.39/scratch/update.txt", std::ios::trunc);
    	
    	if (outputFile.is_open()) {
    		outputFile << "0";
    		
    		outputFile.close();
    	}
    	
    	std::ofstream logFile("/home/ubuntu/ns-allinone-3.39/ns-3.39/scratch/cwnd_log.txt", std::ios::app);
    	
    	if (logFile.is_open()) {
    		logFile << tcb->m_cWnd << std::endl;
    		logFile.close();
    	}
    }
    // END OF TESTING
    
        NS_LOG_INFO("In SlowStart, updated to cwnd " << tcb->m_cWnd << " ssthresh "
                                                     << tcb->m_ssThresh);
        return segmentsAcked - ((tcb->m_cWnd - sndCwnd) / tcb->m_segmentSize);
    }

    return 0;
}

void
TcpLinuxReno::CongestionAvoidance(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
    NS_LOG_FUNCTION(this << tcb << segmentsAcked);

    uint32_t w = tcb->m_cWnd / tcb->m_segmentSize;

    // Floor w to 1 if w == 0
    if (w == 0)
    {
        w = 1;
    }

    NS_LOG_DEBUG("w in segments " << w << " m_cWndCnt " << m_cWndCnt << " segments acked "
                                  << segmentsAcked);
    if (m_cWndCnt >= w)
    {
        m_cWndCnt = 0;
        tcb->m_cWnd += tcb->m_segmentSize;
        NS_LOG_DEBUG("Adding 1 segment to m_cWnd");
    }

    m_cWndCnt += segmentsAcked;
    NS_LOG_DEBUG("Adding 1 segment to m_cWndCnt");
    if (m_cWndCnt >= w)
    {
        uint32_t delta = m_cWndCnt / w;

        m_cWndCnt -= delta * w;
        tcb->m_cWnd += delta * tcb->m_segmentSize;
        NS_LOG_DEBUG("Subtracting delta * w from m_cWndCnt " << delta * w);
    }
    NS_LOG_DEBUG("At end of CongestionAvoidance(), m_cWnd: " << tcb->m_cWnd
                                                             << " m_cWndCnt: " << m_cWndCnt);
	
	// TESTING
    //Open file
	std::ifstream inputFile("/home/ubuntu/ns-allinone-3.39/ns-3.39/scratch/update.txt");
	
	// Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return; // Return an error code
    }
    
    int value;
    inputFile >> value;

	// Check if the read operation was successful
    if (!inputFile) {
        std::cerr << "Error reading from the file." << std::endl;
        return; // Return an error code
    }
    
    // Close the file
    inputFile.close();
    
    if (value == 1) {
    	std::ofstream outputFile("/home/ubuntu/ns-allinone-3.39/ns-3.39/scratch/update.txt", std::ios::trunc);
    	
    	if (outputFile.is_open()) {
    		outputFile << "0";
    		
    		outputFile.close();
    	}
    	
    	std::ofstream logFile("/home/ubuntu/ns-allinone-3.39/ns-3.39/scratch/cwnd_log.txt", std::ios::app);
    	
    	if (logFile.is_open()) {
    		logFile << tcb->m_cWnd << std::endl;
    		logFile.close();
    	}
    }
    //END OF TESTINGs
}

void
TcpLinuxReno::IncreaseWindow(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
    NS_LOG_FUNCTION(this << tcb << segmentsAcked);

    // Linux tcp_in_slow_start() condition
    if (tcb->m_cWnd < tcb->m_ssThresh)
    {
        NS_LOG_DEBUG("In slow start, m_cWnd " << tcb->m_cWnd << " m_ssThresh " << tcb->m_ssThresh);
        segmentsAcked = SlowStart(tcb, segmentsAcked);
    }
    else
    {
        NS_LOG_DEBUG("In cong. avoidance, m_cWnd " << tcb->m_cWnd << " m_ssThresh "
                                                   << tcb->m_ssThresh);
        CongestionAvoidance(tcb, segmentsAcked);
    }
}

std::string
TcpLinuxReno::GetName() const
{
    return "TcpLinuxReno";
}

uint32_t
TcpLinuxReno::GetSsThresh(Ptr<const TcpSocketState> state, uint32_t bytesInFlight)
{
    NS_LOG_FUNCTION(this << state << bytesInFlight);

    // In Linux, it is written as:  return max(tp->snd_cwnd >> 1U, 2U);
    return std::max<uint32_t>(2 * state->m_segmentSize, state->m_cWnd / 2);
}

Ptr<TcpCongestionOps>
TcpLinuxReno::Fork()
{
    return CopyObject<TcpLinuxReno>(this);
}

} // namespace ns3
