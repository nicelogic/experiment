
#include <iostream>

using namespace std;

int main()
{
    cout << "v=0\r\no=libsfusdp 10000 1 IN IP4 0.0.0.0\r\ns=-\r\nt=0 0\r\na=ice-lite\r\na=fingerprint:sha-256 6E:2B:68:FB:D8:AF:60:73:4F:18:12:8A:4F:9D:90:EC:AB:B6:31:C3:F2:5B:0F:21:A 2:E4:4E:52:DE:A6:CF:C5\r\na=msid-semantic: WMS *\r\na=group:BUNDLE 2\r\nm=video 7 RTP/SAVPF 125 107\r\nc=IN IP4 127.0.0.1\r\na=rtpmap:125 H264/90000\r\na=rtpmap:107 rtx/
90000\r\na=fmtp:125 level-asymmetry-allowed=1;pack etization-mode=1;profile-level-id=42e032;x-google-start-bitrate=1000\r\na=fmtp:107 apt=125\r\na=rtcp-fb:125 transport-cc\r\na=rtcp-fb:125 ccm fir\r\na=rtcp-fb:125 nack\r\na=rtcp-fb:125 nack pli\r\na=extmap:1 urn:ietf:pa rams:rtp-hdrext:sdes:mid\r\na=extmap:2 urn:ietf:params:rtp-hdrext:sde
s:rtp-stream-id\r\na=extmap:3 urn:ietf:params:rtp-hdrext:sdes:repaired-rtp-stream-id\r\na=extmap:4 http://www.webrtc.org/experiments/rtp-hdrext/abs-se nd-time\r\na=extmap:5 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01\r\na=extmap:6 http://tools.ietf.org/html/draft-ietf-avtext-framemarking-07\r\na=extmap:11 urn
:3gpp:video-orientation\r\na=e xtmap:12 urn:ietf:params:rtp-hdrext:toffset\r\na=setup:passive\r\na=mid:2\r\na=recvonly\r\na=ice-ufrag:aaul32l7smcmdy70\r\na=ice-pwd:v7ju0nyc5fxzqbop0gwfdkj1iwm0ngdq\r\na=candidate:udpcandidate 1 udp 1076558079 10.67.18 .19 59648 typ host\r\na=end-of-candidates\r\na=ice-options:renomination\r\na=rtcp-mux\r\n
a=rtcp-rsize\r\na=rid:l recv\r\na=rid:m recv\r\na=rid:h recv\r\na=simulcast:recv l;m;h\r\n
" << endl;
    return 0;
}