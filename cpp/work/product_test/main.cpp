
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ofstream ofs("testcfg.pcm", ios::binary);
    for (auto ival = 5, keyID = 0, vmpID = 1, chlID = 1; ival != 97 + 4; ++ival, ++keyID, ++chlID)
    {
        auto realChlID = chlID % 4;
        if(realChlID == 0)
            realChlID = 4;
        char buffer[10000 + 1]{};
        snprintf(buffer, 10000, "<jd6100case1_%u type=\"automediasp\" Timeout=\"0\" Name=\"hsxp\" Description=\"sccsgjjsml，zsccsgjbf\" Path=\"C:\" StartReq=\"S_C_STARTTEST_EX_REQ\" StartAck=\"C_S_STARTTEST_EX_ACK\" StartNack=\"C_S_STARTTEST_EX_NACK\" StartInfo=\"C_S_READY_NOTIFY\" Exception=\"C_S_TEST_EXCEPTION_NOTIFY\" Fail=\"C_S_TESTFAIL_NOTIFY\"  StopTestReq=\"S_C_STOPTEST_REQ\" StopTestAck=\"C_S_STOPTEST_ACK\" StopTestNotify=\"C_S_STOPTEST_NOTIFY\" XMPUType=\"1\" StartRcvReq=\"S_C_STARTRCV_VMP_REQ\" StartRcvAck=\"C_S_STARTRCV_VMP_ACK\" StartRcvNack=\"C_S_STARTRCV_VMP_NACK\" VmpEncNum=\"1\" >\n\
    <EncoderNetParam LocalIP=\"192.168.2.254\" />\n\
    <StreamIn>\n\
      <stream type=\"video\" KeyFrameMsg=\"S_C_FASTUPDATEPIC_CMD\" KeyFrameID=\"%u\" VmpIdx=\"%u\" ChlIdx=\"%u\"/>\n\
    </StreamIn>\n\
    <StreamOut>\n\
    </StreamOut>\n\
    <PreMsg />\n\
    <EndMsg>\n\
      <Msg type=\"Out\" MsgId=\"S_C_STOPRCV_VMP_REQ\" Description=\"S_C_STOPRCV_VMP_REQ qqtzfsml\">\n\
        <MsgBody>\n\
        	<Item type=\"u8\" Value=\"1\" />\n\
        	<Item type=\"u8\" Value=\"%u\" />\n\
        	<Item type=\"u8\" Value=\"%u\" />\n\
        </MsgBody>\n\
      </Msg>\n\
      <Msg type=\"In\" MsgId=\"C_S_STOPRCV_VMP_ACK\" Description=\"C_S_STOPRCV_VMP_ACK tytzfsml\">\n\
        <MsgBody></MsgBody>\n\
      </Msg>\n\
	    <Msg type=\"In\" MsgId=\"C_S_STOPRCV_VMP_NACK\" Description=\"C_S_STOPRCV_VMP_NACK tzfsml\">\n\
        <MsgBody></MsgBody>\n\
      </Msg>\n\
    </EndMsg>\n\
  </jd6100case1_%u>\n",
        ival, keyID, vmpID, realChlID, vmpID, realChlID, ival 
        );
        ofs << buffer;

        if(realChlID == 4)
            ++vmpID;
    }

    ofs.close();
    return 0;
}