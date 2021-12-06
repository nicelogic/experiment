#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <regex>
#include <set>
#include <sstream>
#include <thread>
#include <typeinfo>
#include <vector>

using namespace std;

std::vector<std::string> split(const std::string &s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(std::move(item));
    }
    return elems;
}

std::string getMidFromMlines(const std::vector<std::string> &mLines)
{
    std::string mid;
    for (auto line : mLines)
    {
        const std::string midLineFeature = "a=mid:";
        const auto midLineFeatureIndex = line.find(midLineFeature);
        if (midLineFeatureIndex != std::string::npos)
        {
            mid = line.substr(midLineFeature.length(), line.length() - midLineFeature.length() - std::string("\\r").length());
            break;
        }
    }
    return mid;
}

std::string getStreamIdFromMlines(const std::vector<std::string>& mLines) {
    std::string streamId;
    for(auto line : mLines){
        //a=msid:6B8ED5F6 v6B8ED5F6
        const std::string streamLineFeature = "a=msid:";
        const auto streamLineFeatureIndex = line.find(streamLineFeature);
        const auto spaceIndex = line.find(' ');
        if(streamLineFeatureIndex != std::string::npos
           && spaceIndex != std::string::npos){
            streamId = line.substr(streamLineFeature.length(), spaceIndex - streamLineFeature.length());
            break;
        }
    }
    return streamId;
}

std::vector<std::vector<std::string>> getMediaSectionsLines(const std::string &_sdp)
{
    std::vector<std::vector<std::string>> mSectionsLines;
    const auto sdpLines = split(_sdp, '\n');
    std::vector<std::vector<std::string>> mediaSectionsLines;
    bool hasParsedFirstMLine = false;
    std::vector<std::string> mLines;
    const std::string mLineStartWith = "m=";
    for (auto line : sdpLines)
    {

        if (line.length() >= mLineStartWith.length() && line.substr(0, mLineStartWith.length()) == mLineStartWith)
        {
            if (hasParsedFirstMLine)
            {
                mediaSectionsLines.push_back(mLines);
            }

            hasParsedFirstMLine = true;
            mLines.clear();
            mLines.push_back(line);
        }
        else if (hasParsedFirstMLine)
        {
            mLines.push_back(line);
        }
    }
    if (!mLines.empty())
    {
        mediaSectionsLines.push_back(mLines);
    }
    return mediaSectionsLines;
}

std::map<std::string, std::vector<std::string>> getMsidMids(const std::string& sdp) {
    std::map<std::string, std::vector<std::string>> streamMids;
    std::vector<std::vector<std::string>> mediaSectionsLines = getMediaSectionsLines(sdp);
    for(auto mLines : mediaSectionsLines){
        const auto mid = getMidFromMlines(mLines);
        //maybe empty
        const auto msid = getStreamIdFromMlines(mLines);
        if(!msid.empty() && !mid.empty()){
            streamMids[msid].push_back(mid);
        }
    }
    return streamMids;
}

int main()
{
    // ifstream ifs("log.txt");
    // if (!ifs.is_open())
    // {
    //     cerr << "fs error" << endl;
    //     return 0;
    // }

    // ostringstream oss;
    // oss << ifs.rdbuf();

    // set<string> createEncoders;
    // {
    //     std::istringstream iss_test(oss.str());
    //     string s(1000, '\0');
    //     while (iss_test.getline(&s[0], 1000))
    //     {
    //         if (s.find("Create compression session") != std::string::npos)
    //         {
    //             s = s.substr(s.find_first_of(","));
    //             s = s.substr(s.find_first_of("111"));
    //             s = s.substr(s.find_first_of(":") + 2);
    //             // cout << s.c_str() << "|" << endl;
    //             createEncoders.insert(s);
    //         }
    //         s.clear();
    //         s.resize(1000);
    //     }
    //     cout << "create encoders size: " << createEncoders.size() << endl;
    //     for(auto s : createEncoders){
    //         cout << s << endl;
    //     }
    // }

    // set<string> releaseEncoders;
    // {
    //     std::istringstream iss_test2(oss.str());
    //     string s2(500, '\0');
    //     while (iss_test2.getline(&s2[0], 500))
    //     {
    //         if (s2.find("Release compression session") != std::string::npos)
    //         {
    //             s2 = s2.substr(s2.find_first_of(","));
    //             s2 = s2.substr(s2.find_first_of("222"));
    //             s2 = s2.substr(s2.find_first_of(":") + 13);
    //             releaseEncoders.insert(s2);
    //         }
    //         s2.clear();
    //         s2.resize(1000);
    //     }
    //     cout << "release encoders size: " << releaseEncoders.size() << endl;
    //     for(auto s : releaseEncoders){
    //         cout << s << endl;
    //     }
    // }

    // vector<string> diffs;
    // std::set_difference(createEncoders.begin(), createEncoders.end(), releaseEncoders.begin(), releaseEncoders.end(),
    //                     std::back_inserter(diffs));
    // cout << diffs.size() << endl;

    //     ifs.close();

    string _sdp = R"(
        o=- 1935423972072288406 6 IN IP4 127.0.0.1
s=-
t=0 0
a=group:BUNDLE 0 1 2 3 4
a=extmap-allow-mixed
a=msid-semantic: WMS 88093079
a=rcv-pc-role:main
a=rcv-p2p
a=rcv-mix-audio:recv-mode=auto
a=rcv-negotiate-video:recv-mode=auto
m=video 56492 UDP/TLS/RTP/SAVPF 127 97 98 99 100 101 125
c=IN IP4 127.0.0.1
a=rtcp:9 IN IP4 0.0.0.0
a=candidate:537412976 1 udp 2122260224 192.168.2.7 62235 typ host generation 0 network-id 1 network-cost 10
a=candidate:594946817 1 udp 2122194688 169.254.220.183 59476 typ host generation 0 network-id 2 network-cost 10
a=candidate:229727961 1 udp 2122129152 10.32.49.160 54964 typ host generation 0 network-id 5 network-cost 50
a=candidate:1110780486 1 udp 24977152 127.0.0.1 56492 typ relay raddr 10.32.49.160 rport 55122 generation 0 network-id 5 network-cost 50
a=candidate:1554278195 1 udp 8199936 127.0.0.1 61498 typ relay raddr 10.32.49.160 rport 55121 generation 0 network-id 5 network-cost 50
a=candidate:1854650752 1 tcp 1518280448 192.168.2.7 55130 typ host tcptype passive generation 0 network-id 1 network-cost 10
a=candidate:1844828145 1 tcp 1518214912 169.254.220.183 55131 typ host tcptype passive generation 0 network-id 2 network-cost 10
a=candidate:1127391785 1 tcp 1518149376 10.32.49.160 55132 typ host tcptype passive generation 0 network-id 5 network-cost 50
a=candidate:537412976 1 udp 2122260223 192.168.2.7 56976 typ host generation 0 network-id 1 network-cost 10
a=candidate:594946817 1 udp 2122194687 169.254.220.183 49320 typ host generation 0 network-id 2 network-cost 10
a=ice-ufrag:BxdJ
a=ice-pwd:K5FCXWV1gkW3cNzm9LDZVVt0
a=ice-options:trickle renomination
a=fingerprint:sha-256 29:A9:69:A5:53:30:4E:42:85:FD:97:49:C5:4F:4D:EA:29:B0:C3:5F:D3:A6:79:C7:C3:0B:E0:3E:98:0B:ED:4E
a=setup:actpass
a=mid:0
a=extmap:1 urn:ietf:params:rtp-hdrext:toffset
a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
a=extmap:3 urn:3gpp:video-orientation
a=extmap:4 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
a=extmap:5 http://www.webrtc.org/experiments/rtp-hdrext/playout-delay
a=extmap:6 http://www.webrtc.org/experiments/rtp-hdrext/video-content-type
a=extmap:7 http://www.webrtc.org/experiments/rtp-hdrext/video-timing
a=extmap:8 http://www.webrtc.org/experiments/rtp-hdrext/color-space
a=extmap:9 urn:ietf:params:rtp-hdrext:sdes:mid
a=extmap:10 urn:ietf:params:rtp-hdrext:sdes:rtp-stream-id
a=extmap:11 urn:ietf:params:rtp-hdrext:sdes:repaired-rtp-stream-id
a=sendrecv
a=msid:88093079 v88093079
a=rtcp-mux
a=rtcp-rsize
a=rtpmap:127 H264/90000
a=rtcp-fb:127 goog-remb
a=rtcp-fb:127 transport-cc
a=rtcp-fb:127 ccm fir
a=rtcp-fb:127 nack
a=rtcp-fb:127 nack pli
a=fmtp:127 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f
a=rtpmap:97 rtx/90000
a=fmtp:97 apt=127
a=rtpmap:98 VP8/90000
a=rtcp-fb:98 goog-remb
a=rtcp-fb:98 transport-cc
a=rtcp-fb:98 ccm fir
a=rtcp-fb:98 nack
a=rtcp-fb:98 nack pli
a=rtpmap:99 rtx/90000
a=fmtp:99 apt=98
a=rtpmap:100 red/90000
a=rtpmap:101 rtx/90000
a=fmtp:101 apt=100
a=rtpmap:125 ulpfec/90000
a=ssrc-group:FID 2458190670 2417878222
a=ssrc-group:FID 646948487 212500069
a=ssrc-group:FID 903134696 646834366
a=ssrc-group:SIM 2458190670 646948487 903134696
a=ssrc:2458190670 cname:DJcMoo6dRY+cGLHV
a=ssrc:2458190670 msid:88093079 v88093079
a=ssrc:2458190670 mslabel:88093079
a=ssrc:2458190670 label:v88093079
a=ssrc:2417878222 cname:DJcMoo6dRY+cGLHV
a=ssrc:2417878222 msid:88093079 v88093079
a=ssrc:2417878222 mslabel:88093079
a=ssrc:2417878222 label:v88093079
a=ssrc:646948487 cname:DJcMoo6dRY+cGLHV
a=ssrc:646948487 msid:88093079 v88093079
a=ssrc:646948487 mslabel:88093079
a=ssrc:646948487 label:v88093079
a=ssrc:212500069 cname:DJcMoo6dRY+cGLHV
a=ssrc:212500069 msid:88093079 v88093079
a=ssrc:212500069 mslabel:88093079
a=ssrc:212500069 label:v88093079
a=ssrc:903134696 cname:DJcMoo6dRY+cGLHV
a=ssrc:903134696 msid:88093079 v88093079
a=ssrc:903134696 mslabel:88093079
a=ssrc:903134696 label:v88093079
a=ssrc:646834366 cname:DJcMoo6dRY+cGLHV
a=ssrc:646834366 msid:88093079 v88093079
a=ssrc:646834366 mslabel:88093079
a=ssrc:646834366 label:v88093079
m=audio 51524 UDP/TLS/RTP/SAVPF 111 103 104 9 102 0 8 106 105 13 110 112 113 126
c=IN IP4 127.0.0.1
a=rtcp:9 IN IP4 0.0.0.0
a=candidate:537412976 1 udp 2122260224 192.168.2.7 49226 typ host generation 0 network-id 1 network-cost 10
a=candidate:594946817 1 udp 2122194688 169.254.220.183 61243 typ host generation 0 network-id 2 network-cost 10
a=candidate:229727961 1 udp 2122129152 10.32.49.160 59068 typ host generation 0 network-id 5 network-cost 50
a=candidate:1110780486 1 udp 24977152 127.0.0.1 51524 typ relay raddr 10.32.49.160 rport 55129 generation 0 network-id 5 network-cost 50
a=candidate:1554278195 1 udp 8199936 127.0.0.1 50024 typ relay raddr 10.32.49.160 rport 55128 generation 0 network-id 5 network-cost 50
a=candidate:1854650752 1 tcp 1518280448 192.168.2.7 55133 typ host tcptype passive generation 0 network-id 1 network-cost 10
a=candidate:1844828145 1 tcp 1518214912 169.254.220.183 55134 typ host tcptype passive generation 0 network-id 2 network-cost 10
a=candidate:1127391785 1 tcp 1518149376 10.32.49.160 55135 typ host tcptype passive generation 0 network-id 5 network-cost 50
a=ice-ufrag:BxdJ
a=ice-pwd:K5FCXWV1gkW3cNzm9LDZVVt0
a=ice-options:trickle renomination
a=fingerprint:sha-256 29:A9:69:A5:53:30:4E:42:85:FD:97:49:C5:4F:4D:EA:29:B0:C3:5F:D3:A6:79:C7:C3:0B:E0:3E:98:0B:ED:4E
a=setup:actpass
a=mid:1
a=extmap:14 urn:ietf:params:rtp-hdrext:ssrc-audio-level
a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
a=extmap:4 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
a=extmap:9 urn:ietf:params:rtp-hdrext:sdes:mid
a=extmap:10 urn:ietf:params:rtp-hdrext:sdes:rtp-stream-id
a=extmap:11 urn:ietf:params:rtp-hdrext:sdes:repaired-rtp-stream-id
a=sendrecv
a=msid:88093079 a88093079
a=rtcp-mux
a=rtpmap:111 opus/48000/2
a=rtcp-fb:111 transport-cc
a=fmtp:111 minptime=10;useinbandfec=1
a=rtpmap:103 ISAC/16000
a=rtpmap:104 ISAC/32000
a=rtpmap:9 G722/8000
a=rtpmap:102 ILBC/8000
a=rtpmap:0 PCMU/8000
a=rtpmap:8 PCMA/8000
a=rtpmap:106 CN/32000
a=rtpmap:105 CN/16000
a=rtpmap:13 CN/8000
a=rtpmap:110 telephone-event/48000
a=rtpmap:112 telephone-event/32000
a=rtpmap:113 telephone-event/16000
a=rtpmap:126 telephone-event/8000
a=ssrc:1748232190 cname:DJcMoo6dRY+cGLHV
a=ssrc:1748232190 msid:88093079 a88093079
a=ssrc:1748232190 mslabel:88093079
a=ssrc:1748232190 label:a88093079
m=application 9 UDP/DTLS/SCTP webrtc-datachannel
c=IN IP4 0.0.0.0
a=ice-ufrag:BxdJ
a=ice-pwd:K5FCXWV1gkW3cNzm9LDZVVt0
a=ice-options:trickle renomination
a=fingerprint:sha-256 29:A9:69:A5:53:30:4E:42:85:FD:97:49:C5:4F:4D:EA:29:B0:C3:5F:D3:A6:79:C7:C3:0B:E0:3E:98:0B:ED:4E
a=setup:actpass
a=mid:2
a=sctp-port:5000
a=max-message-size:262144
m=video 62696 UDP/TLS/RTP/SAVPF 127 97 98 99 100 101 125 124 35 36 37
c=IN IP4 192.168.2.7
a=rtcp:9 IN IP4 0.0.0.0
a=candidate:537412976 1 udp 2122260223 192.168.2.7 62696 typ host generation 0 network-id 1 network-cost 10
a=candidate:594946817 1 udp 2122194687 169.254.220.183 53755 typ host generation 0 network-id 2 network-cost 10
a=candidate:229727961 1 udp 2122129151 10.32.49.160 58462 typ host generation 0 network-id 5 network-cost 50
a=ice-ufrag:BxdJ
a=ice-pwd:K5FCXWV1gkW3cNzm9LDZVVt0
a=ice-options:trickle renomination
a=fingerprint:sha-256 29:A9:69:A5:53:30:4E:42:85:FD:97:49:C5:4F:4D:EA:29:B0:C3:5F:D3:A6:79:C7:C3:0B:E0:3E:98:0B:ED:4E
a=setup:actpass
a=mid:3
a=extmap:1 urn:ietf:params:rtp-hdrext:toffset
a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
a=extmap:3 urn:3gpp:video-orientation
a=extmap:4 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
a=extmap:5 http://www.webrtc.org/experiments/rtp-hdrext/playout-delay
a=extmap:6 http://www.webrtc.org/experiments/rtp-hdrext/video-content-type
a=extmap:7 http://www.webrtc.org/experiments/rtp-hdrext/video-timing
a=extmap:8 http://www.webrtc.org/experiments/rtp-hdrext/color-space
a=extmap:9 urn:ietf:params:rtp-hdrext:sdes:mid
a=extmap:10 urn:ietf:params:rtp-hdrext:sdes:rtp-stream-id
a=extmap:11 urn:ietf:params:rtp-hdrext:sdes:repaired-rtp-stream-id
a=recvonly
a=msid:9A7D9715 s9A7D9715
a=rtcp-mux
a=rtcp-rsize
a=rtpmap:127 H264/90000
a=rtcp-fb:127 goog-remb
a=rtcp-fb:127 transport-cc
a=rtcp-fb:127 ccm fir
a=rtcp-fb:127 nack
a=rtcp-fb:127 nack pli
a=fmtp:127 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f
a=rtpmap:97 rtx/90000
a=fmtp:97 apt=127
a=rtpmap:98 VP8/90000
a=rtcp-fb:98 goog-remb
a=rtcp-fb:98 transport-cc
a=rtcp-fb:98 ccm fir
a=rtcp-fb:98 nack
a=rtcp-fb:98 nack pli
a=rtpmap:99 rtx/90000
a=fmtp:99 apt=98
a=rtpmap:100 red/90000
a=rtpmap:101 rtx/90000
a=fmtp:101 apt=100
a=rtpmap:125 ulpfec/90000
a=rtpmap:124 H264/90000
a=rtcp-fb:124 goog-remb
a=rtcp-fb:124 transport-cc
a=rtcp-fb:124 ccm fir
a=rtcp-fb:124 nack
a=rtcp-fb:124 nack pli
a=fmtp:124 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=640c29
a=rtpmap:35 AV1X/90000
a=rtcp-fb:35 goog-remb
a=rtcp-fb:35 transport-cc
a=rtcp-fb:35 ccm fir
a=rtcp-fb:35 nack
a=rtcp-fb:35 nack pli
a=rtpmap:36 rtx/90000
a=fmtp:36 apt=35
a=rtpmap:37 flexfec-03/90000
a=rtcp-fb:37 goog-remb
a=rtcp-fb:37 transport-cc
a=fmtp:37 repair-window=10000000
a=ssrc-group:FID 71962109 2853010331
a=ssrc:71962109 cname:DJcMoo6dRY+cGLHV
a=ssrc:71962109 msid:9A7D9715 s9A7D9715
a=ssrc:71962109 mslabel:9A7D9715
a=ssrc:71962109 label:s9A7D9715
a=ssrc:2853010331 cname:DJcMoo6dRY+cGLHV
a=ssrc:2853010331 msid:9A7D9715 s9A7D9715
a=ssrc:2853010331 mslabel:9A7D9715
a=ssrc:2853010331 label:s9A7D9715
m=video 9 UDP/TLS/RTP/SAVPF 127 97 98 99 100 101 125
c=IN IP4 0.0.0.0
a=rtcp:9 IN IP4 0.0.0.0
a=ice-ufrag:BxdJ
a=ice-pwd:K5FCXWV1gkW3cNzm9LDZVVt0
a=ice-options:trickle renomination
a=fingerprint:sha-256 29:A9:69:A5:53:30:4E:42:85:FD:97:49:C5:4F:4D:EA:29:B0:C3:5F:D3:A6:79:C7:C3:0B:E0:3E:98:0B:ED:4E
a=setup:actpass
a=mid:4
a=extmap:1 urn:ietf:params:rtp-hdrext:toffset
a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
a=extmap:3 urn:3gpp:video-orientation
a=extmap:4 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
a=extmap:5 http://www.webrtc.org/experiments/rtp-hdrext/playout-delay
a=extmap:6 http://www.webrtc.org/experiments/rtp-hdrext/video-content-type
a=extmap:7 http://www.webrtc.org/experiments/rtp-hdrext/video-timing
a=extmap:8 http://www.webrtc.org/experiments/rtp-hdrext/color-space
a=extmap:9 urn:ietf:params:rtp-hdrext:sdes:mid
a=extmap:10 urn:ietf:params:rtp-hdrext:sdes:rtp-stream-id
a=extmap:11 urn:ietf:params:rtp-hdrext:sdes:repaired-rtp-stream-id
a=sendrecv
a=msid:A1B14269 sAAAAAAAA
a=rtcp-mux
a=rtcp-rsize
a=rtpmap:127 H264/90000
a=rtcp-fb:127 goog-remb
a=rtcp-fb:127 transport-cc
a=rtcp-fb:127 ccm fir
a=rtcp-fb:127 nack
a=rtcp-fb:127 nack pli
a=fmtp:127 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e029
a=rtpmap:97 rtx/90000
a=fmtp:97 apt=127
a=rtpmap:98 VP8/90000
a=rtcp-fb:98 goog-remb
a=rtcp-fb:98 transport-cc
a=rtcp-fb:98 ccm fir
a=rtcp-fb:98 nack
a=rtcp-fb:98 nack pli
a=rtpmap:99 rtx/90000
a=fmtp:99 apt=98
a=rtpmap:100 red/90000
a=rtpmap:101 rtx/90000
a=fmtp:101 apt=100
a=rtpmap:125 ulpfec/90000
a=ssrc-group:FID 1458023372 1134522239
a=ssrc:1458023372 cname:DJcMoo6dRY+cGLHV
a=ssrc:1458023372 msid:A1B14269 sA1B14269
a=ssrc:1458023372 mslabel:A1B14269
a=ssrc:1458023372 label:sA1B14269
a=ssrc:1134522239 cname:DJcMoo6dRY+cGLHV
a=ssrc:1134522239 msid:A1B14269 sA1B14269
a=ssrc:1134522239 mslabel:A1B14269
a=ssrc:1134522239 label:sA1B14269
a=x-google-flag:conference
        )";
    // const auto a = getMsidMids(_sdp);
    // for(auto i : a){
    //     cout << i.first << endl;
    //     for(auto j : i.second){
    //         cout << j << endl;
    //     }
    // }
            //_sdp = std::regex_replace(_sdp, std::regex("42e029"), "42e01f");
            //cout << _sdp << endl;
            const auto iter = _sdp.find(std::string("a=msid:") + "88093079" + " s");
            cout << (iter != std::string::npos) << endl;


const auto count = 5;



const string sdp2 = "v=0\r\no=118801 406415273958451200 4 IN IP4 127.0.0.1\r\ns=RC SFU\r\nt=0 0\r\na=rcv-remote-session:4643076192371717542\r\na=rcv-pc-role:main\r\na=rcv-denoise:enabled=1\r\na=rcv-agc:enabled=1\r\na=rcv-noise-gate:enabled=1\r\na=rcv-mix-audio:send-mode=auto;enabled=1\r\na=rcv-negotiate-video:send-mode=always;enabled=1\r\na=group:BUNDLE 0 1 2 3\r\na=msid-semantic: WMS 118793-40b8725d3b0a5ed6325654bc2cb6a760\r\na=ice-ufrag:2jtkR7LP8Z0zmaSI\r\na=ice-pwd:Eta3tox6UMuvnIQBXIfqwO9s\r\nm=video 9 UDP/TLS/RTP/SAVPF 127 98 100 125 99 101 97\r\nc=IN IP4 0.0.0.0\r\nb=AS:2500\r\na=mid:0\r\na=recvonly\r\na=ice-options:trickle\r\na=rtcp-mux\r\na=maxptime:60\r\na=setup:actpass\r\na=ice-ufrag:2jtkR7LP8Z0zmaSI\r\na=ice-pwd:Eta3tox6UMuvnIQBXIfqwO9s\r\na=fingerprint:sha-256 09:E3:7A:90:48:06:07:FE:23:F1:1D:79:6C:63:62:8B:9B:95:29:8D:42:25:E3:F2:BD:6A:AE:B7:E2:12:7F:2E\r\na=extmap:1 urn:ietf:params:rtp-hdrext:toffset\r\na=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time\r\na=extmap:4 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01\r\na=x-google-flag:conference\r\na=rtpmap:127 H264/90000\r\na=rtpmap:98 VP8/90000\r\na=rtpmap:100 RED/90000\r\na=rtpmap:125 ULPFEC/90000\r\na=rtpmap:99 RTX/90000\r\na=rtpmap:101 RTX/90000\r\na=rtpmap:97 RTX/90000\r\na=rtcp-fb:127 ccm fir\r\na=rtcp-fb:127 nack\r\na=rtcp-fb:127 nack pli\r\na=rtcp-fb:127 transport-cc\r\na=rtcp-fb:98 ccm fir\r\na=rtcp-fb:98 nack\r\na=rtcp-fb:98 nack pli\r\na=rtcp-fb:98 transport-cc\r\na=fmtp:127 profile-level-id=42e01f;level-asymmetry-allowed=1;packetization-mode=1;sps-pps-idr-in-keyframe=1\r\na=fmtp:98 max-fs=12288;max-fr=60\r\na=fmtp:99 apt=98\r\na=fmtp:101 apt=100\r\na=fmtp:97 apt=127\r\na=ssrc:752408 cname:AJbpH7LEZ1DwiuVO\r\nm=audio 9 UDP/TLS/RTP/SAVPF 111 106 105 13\r\nc=IN IP4 0.0.0.0\r\na=mid:1\r\na=sendrecv\r\na=ice-options:trickle\r\na=rtcp-mux\r\na=maxptime:60\r\na=setup:actpass\r\na=ice-ufrag:2jtkR7LP8Z0zmaSI\r\na=ice-pwd:Eta3tox6UMuvnIQBXIfqwO9s\r\na=fingerprint:sha-256 09:E3:7A:90:48:06:07:FE:23:F1:1D:79:6C:63:62:8B:9B:95:29:8D:42:25:E3:F2:BD:6A:AE:B7:E2:12:7F:2E\r\na=extmap:4 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01\r\na=extmap:14 urn:ietf:params:rtp-hdrext:ssrc-audio-level\r\na=rtpmap:111 OPUS/48000/2\r\na=rtpmap:106 CN/32000\r\na=rtpmap:105 CN/16000\r\na=rtpmap:13 CN/8000\r\na=rtcp-fb:111 transport-cc\r\na=fmtp:111 minptime=10;useinbandfec=1;maxplaybackrate=48000;sprop-maxcapturerate=48000;maxaveragebitrate=32000;stereo=0;usedtx=1\r\na=msid:mix-stream mix-audio\r\na=ssrc:752409 msid:mix-stream mix-audio\r\na=ssrc:752409 mslabel:mix-stream\r\na=ssrc:752409 label:mix-audio\r\na=ssrc:752409 cname:AJbpH7LEZ1DwiuVO\r\nm=application 9 UDP/DTLS/SCTP webrtc-datachannel\r\nc=IN IP4 0.0.0.0\r\na=mid:2\r\na=sendrecv\r\na=ice-options:trickle\r\na=setup:actpass\r\na=ice-ufrag:2jtkR7LP8Z0zmaSI\r\na=ice-pwd:Eta3tox6UMuvnIQBXIfqwO9s\r\na=fingerprint:sha-256 09:E3:7A:90:48:06:07:FE:23:F1:1D:79:6C:63:62:8B:9B:95:29:8D:42:25:E3:F2:BD:6A:AE:B7:E2:12:7F:2E\r\na=sctp-port:5000\r\nm=video 9 UDP/TLS/RTP/SAVPF 98 100 125 99 101\r\nc=IN IP4 0.0.0.0\r\nb=AS:2500\r\na=mid:3\r\na=sendrecv\r\na=ice-options:trickle\r\na=rtcp-mux\r\na=maxptime:60\r\na=setup:actpass\r\na=ice-ufrag:2jtkR7LP8Z0zmaSI\r\na=ice-pwd:Eta3tox6UMuvnIQBXIfqwO9s\r\na=fingerprint:sha-256 09:E3:7A:90:48:06:07:FE:23:F1:1D:79:6C:63:62:8B:9B:95:29:8D:42:25:E3:F2:BD:6A:AE:B7:E2:12:7F:2E\r\na=extmap:2 urn:ietf:params:rtp-hdrext:toffset\r\na=extmap:3 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time\r\na=extmap:5 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01\r\na=x-google-flag:conference\r\na=rtpmap:98 VP8/90000\r\na=rtpmap:100 RED/90000\r\na=rtpmap:125 ULPFEC/90000\r\na=rtpmap:99 RTX/90000\r\na=rtpmap:101 RTX/90000\r\na=rtcp-fb:98 ccm fir\r\na=rtcp-fb:98 nack\r\na=rtcp-fb:98 nack pli\r\na=rtcp-fb:98 transport-cc\r\na=fmtp:98 max-fs=12288;max-fr=60\r\na=fmtp:99 apt=98\r\na=fmtp:101 apt=100\r\na=msid:118793-cf6b2177d7724d2ad5be4c6dccd8eaad 118793-15c92287-56c0-40a5-a1fc-e65aabe57252-752419\r\na=ssrc:752419 msid:118793-cf6b2177d7724d2ad5be4c6dccd8eaad 118793-15c92287-56c0-40a5-a1fc-e65aabe57252-752419\r\na=ssrc:752419 mslabel:118793-cf6b2177d7724d2ad5be4c6dccd8eaad\r\na=ssrc:752419 label:118793-15c92287-56c0-40a5-a1fc-e65aabe57252-752419\r\na=ssrc:752419 cname:118793-hz+pvfcM2r1vEA8u\r\na=ssrc:752421 cname:118793-hz+pvfcM2r1vEA8u\r\na=ssrc:752421 msid:118793-cf6b2177d7724d2ad5be4c6dccd8eaad 118793-15c92287-56c0-40a5-a1fc-e65aabe57252-752419\r\na=ssrc:752421 mslabel:118793-cf6b2177d7724d2ad5be4c6dccd8eaad\r\na=ssrc:752421 label:118793-15c92287-56c0-40a5-a1fc-e65aabe57252-752419\r\na=ssrc-group:FID 752419 752421\r\n";

    // auto a = sdp2.rfind("mslabel");
    // auto sub = sdp2.substr(a + 8, 39);
    // cout << sub << endl;

    // static std::regex re("\\na=ssrc:(\\S*)\\smslabel:(\\S*)", std::regex_constants::ECMAScript | std::regex_constants::icase);
    // std::smatch match;
    // std::vector<std::string> result;
    // for (auto sdpIter = _sdp.cbegin();
    //      std::regex_search(sdpIter, _sdp.cend(), match, re);
    //      sdpIter += (match.position(0) + match.length(0)))
    // {
    //     const auto matchStr = match[2].str();
    //     if (!matchStr.empty()
    //     && std::find(result.begin(), result.end(), matchStr) == result.end())
    //     {
    //         result.push_back(match[2].str());
    //     }
    // }
    // for(auto msid : result){
    //     cout << msid << endl;
    // }

    // string s = "a=msid:6B8ED5F6 v6B8ED5F6\r";
    // auto index = s.find("a=msid:");
    // auto index2 = s.find(' ');
    // cout << index << endl;
    // string mid;
    // if (index != string::npos && index2 != string::npos)
    // {
    //     mid = s.substr(std::string("a=msid:").length(), index2 - std::string("a=msid:").length());
    // }
    // cout << mid << "|" << endl;

    // const std::string midLineFeature = "a=mid:";
    // const string line = "a=mid:dfasa\\r";
    // if (line.find(midLineFeature) != std::string::npos) {
    //     mid = line.substr(midLineFeature.length(), line.length() - midLineFeature.length() - std::string("\\r").length());
    //     cout << mid << endl;
    // }

    // auto z = getMediaSectionsLines(_sdp);
    // cout << z.size() << endl;
    // cout << "------" << endl;
    // for (auto ms : z)
    // {
    //     cout << "------" << endl;
    //     auto mid =  getMidFromMlines(ms);
    //     cout << mid << endl;
    //     for (auto msi : ms)
    //     {
    //         // cout << msi << endl;
    //     }
    // }
}