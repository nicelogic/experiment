
#include <algorithm>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <regex>



#include <cassert>
#include <sstream>
#include <regex>
#include <random>
#include <iomanip>


#include "./aoffer.hpp"
#include "./boffer.hpp"

using namespace std;

namespace
{
    const size_t not_found = std::numeric_limits<size_t>::max();
    static const std::string TAG = "pc_sdp_util";


    std::vector<std::string> split(const std::string &s, char delim) {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delim))
        {
            elems.push_back(std::move(item));
        }
        return elems;
    }

    std::string join(const std::vector<std::string> &lines, std::string separator = "\n")
    {
        std::string result;
        size_t size = 0;
        for (const auto &l: lines) size += l.size();
        result.reserve(lines.size() * separator.size() + size + 1);

        for (size_t i = 0; i < lines.size(); ++i)
        {
            if (i)
            {
                result += separator;
            }
            result += lines[i];
        }
        return result;
    }

    void push_back_not_empty(std::vector<std::string> &lines, std::string value)
    {
        if (value.size())
        {
            lines.emplace_back(value);
        }
    }

    bool parse_mline(const std::string &line, std::string &type)
    {
        static std::regex re("^m=(\\S+) ", std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch match;
        if (std::regex_search(line, match, re))
        {
            type = match[1].str();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool parse_fid(const std::string &line, std::string &ssrc, std::string &ssrc_fid)
    {
        static std::regex re("a=ssrc-group:FID (\\d+) (\\d+)", std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch match;
        if (std::regex_match(line, match, re))
        {
            ssrc = match[1].str();
            ssrc_fid = match[2].str();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool parse_cname(const std::string &line, std::string &ssrc, std::string &cname)
    {
        static std::regex re("a=ssrc:(\\d+) cname:(\\S+)", std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch match;
        if (std::regex_match(line, match, re))
        {
            ssrc = match[1].str();
            cname = match[2].str();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool parse_mslabel(const std::string &line, std::string &ssrc, std::string &mslabel)
    {
        static std::regex re("a=ssrc:(\\d+) mslabel:(\\S+)", std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch match;
        if (std::regex_match(line, match, re))
        {
            ssrc = match[1].str();
            mslabel = match[2].str();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool parse_label(const std::string &line, std::string &ssrc, std::string &label)
    {
        static std::regex re("a=ssrc:(\\d+) label:(\\S+)", std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch match;
        if (std::regex_match(line, match, re))
        {
            ssrc = match[1].str();
            label = match[2].str();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool parse_rtp_map(const std::string &line, std::string &payload, std::string &codec)
    {
        static std::regex re("^a=rtpmap:(\\d+) (\\S+)/\\d+", std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch match;
        if (std::regex_search(line, match, re))
        {
            payload = match[1].str();
            codec = match[2].str();
            return true;
        }
        else
        {
            return false;
        }
    }

    std::string gen_ssrc()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<unsigned long> dis(1, 999999999);

        const auto value = dis(gen);
        std::stringstream result;
        result << std::setw(9) << std::setfill('0');
        result << value;
        return result.str();
    }

    void inject_ssrc(std::vector<std::string> &lines, const std::string &ssrc, const std::string &cname, const std::string &mslabel, const std::string &label)
    {
        const std::string assrc = "a=ssrc:"   + ssrc;
        lines.emplace_back(assrc + " cname:"   + cname   + "\r");
        lines.emplace_back(assrc + " msid:"    + mslabel + " " + label + "\r");
        lines.emplace_back(assrc + " mslabel:" + mslabel + "\r");
        lines.emplace_back(assrc + " label:"   + label   + "\r");
    }

    std::string inject_ssrc_and_fid(std::vector<std::string> &lines, const std::string &cname, const std::string &mslabel, const std::string &label, bool fid)
    {
        const auto ssrc = gen_ssrc();
        const auto ssrc_fid = gen_ssrc();
        if (fid) {
            lines.push_back("a=ssrc-group:FID " + ssrc + " " + ssrc_fid + "\r");
        }
        inject_ssrc(lines, ssrc, cname, mslabel, label);
        if (fid) {
            inject_ssrc(lines, ssrc_fid, cname, mslabel, label);
        }
        return ssrc;
    }

    std::vector<std::string>::iterator rcv_a_it(std::vector<std::string> &lines)
    {
        static std::regex re("^a=msid-semantic:\\sWMS(\\s\\S+)*\\r$", std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch match;
        bool found = false;

        for (auto it = lines.begin(); it != lines.end(); ++it)
        {
            if (found)
            {
                return it;
            }
            if (std::regex_search(*it, match, re))
            {
                found = true;
            }
        }
        return lines.end();
    }

    struct opus_scan_result
    {
        std::string payload;
        std::vector<std::string> additional_payloads;
        size_t audio_mLineIdx;
    };

    
}

 class SDPMungler
    {
    public:
        explicit SDPMungler(const std::string &sdp);
        void inject_conference(bool value);
        void inject_track_info(const std::string &trackId, int width, int height);
        void inject_simulcast(int64_t extraSSRC);
        void inject_p2p(bool value);
        void inject_pc_role(const std::string &value);
        void inject_parties(const std::string &local, const std::string &remote);
        void inject_remote_session(const std::string &value);
        void prefer_codec(const std::string &codec);
        void prefer_audio_codec(const std::string &codec);
        std::string result() const;
        std::string result(std::string *minified) const;
        void set_previous_sdp(const std::string &localSDP);
        void set_propagate_rotation(bool);
        void set_audio_inactive(bool);
        void set_mix_audio_recv(const std::string &recv_mode);
        void set_negotiate_video_recv(const std::string &recv_mode);
        void set_denoise_at_sfu(bool enabled);
        void remove_audio_red(bool enbaled);
        
    private:
        std::string rcv_track() const;
        std::string rcv_pc_role() const;
        std::string rcv_p2p() const;
        std::string rcv_local_party() const;
        std::string rcv_remote_party() const;
        std::string rcv_remote_session() const;
        std::string rcv_mix_audio() const;
        std::string rcv_negotiate_video() const;
        bool p2p_sdp() const;
        std::string rcv_denoise_at_sfu() const;

        void inject_rcv(std::vector<std::string> &) const;
        void inject_simulcast(std::vector<std::string> &) const;
        void inject_simulcast_by_unified_plan(std::vector<std::string> &) const;
        void inject_conference(std::vector<std::string> &) const;
        std::vector<std::string> minify(const std::vector<std::string> &) const;

    private:
        const std::vector<std::string> _lines;
        bool _inject_conference = false;
        std::string _rcv_track_id;
        int _rcv_track_width = 0;
        int _rcv_track_height = 0;
        int64_t _simulcast = 0;
        bool _p2p = false;
        std::string _pc_role;
        std::string _local_party;
        std::string _remote_party;
        std::string _remote_session;
        std::string _prefer_codec;
        std::string _prefer_audio_codec;
        std::string _previousSDP;
        bool _propagate_rotation = false;
        bool _inactive_audio = false;
        std::string _mix_audio_recv;
        std::string _negotiate_video_recv;
        bool _denoise_at_sfu = true;
        bool _remove_audio_red = false;

    };



    SDPMungler::SDPMungler(const std::string &sdp)
    : _lines(split(sdp, '\n'))
    {

    }

    void SDPMungler::inject_conference(bool value)
    {
        _inject_conference = value;
    }

    void SDPMungler::inject_track_info(const std::string &trackId, int width, int height)
    {
        _rcv_track_id = trackId;
        _rcv_track_width = width;
        _rcv_track_height = height;
    }

    void SDPMungler::inject_simulcast(int64_t extraSSRC)
    {
        _simulcast = extraSSRC;
    }

    void SDPMungler::inject_p2p(bool value)
    {
        _p2p = value;
    }

    void SDPMungler::inject_pc_role(const std::string &value)
    {
        _pc_role = value;
    }

    void SDPMungler::inject_parties(const std::string &local, const std::string &remote)
    {
        _local_party = local;
        _remote_party = remote;
        assert((_local_party.size() && _remote_party.size()) || (_local_party.empty() && _remote_party.empty()));
    }

    void SDPMungler::inject_remote_session(const std::string &value)
    {
        _remote_session = value;
    }


    
    void SDPMungler::set_previous_sdp(const std::string &localSDP) {
        _previousSDP = localSDP;
    }

    void SDPMungler::set_propagate_rotation(bool value) {
        _propagate_rotation = value;
    }
    
    void SDPMungler::set_audio_inactive(bool value) {
        _inactive_audio = value;
    }
    
    void SDPMungler::set_mix_audio_recv(const std::string &recv_mode) {
        _mix_audio_recv = recv_mode;
    }
    
    void SDPMungler::set_negotiate_video_recv(const std::string &recv_mode) {
        _negotiate_video_recv = recv_mode;
    }

    void SDPMungler::set_denoise_at_sfu(bool enbaled)
    {
        _denoise_at_sfu = enbaled;
    }

    void SDPMungler::remove_audio_red(bool enbaled)
    {
        _remove_audio_red = enbaled;
    }

    std::string SDPMungler::result(std::string *minified) const {
        if (p2p_sdp())
        {
            // Either mungle for p2p or declare p2p availability
            assert(_p2p == 0);
            // Either mungle for p2p or declare simulcast
            assert(_simulcast == 0);
        }

        auto lines = _lines;
        cout << "lines num: " << lines.size() << endl;

        inject_rcv(lines);
        inject_simulcast_by_unified_plan(lines);
        cout << "inject sim end" << endl;
        inject_conference(lines);
        
        lines.emplace_back("");

        std::string result = join(lines);
        return result;
    }

    std::string SDPMungler::result() const
    {
        return result(nullptr);
    }

    std::string SDPMungler::rcv_track() const
    {
        if (_rcv_track_id.empty() || (_rcv_track_width == 0 && _rcv_track_height == 0))
        {
            return "";
        }
        std::stringstream ss;
        ss << "a=rcv-track:" << _rcv_track_id << " " << _rcv_track_width << " " << _rcv_track_height << "\r";
        return ss.str();
    }

    std::string SDPMungler::rcv_pc_role() const
    {
        if (_pc_role.empty())
        {
            return "";
        }
        return "a=rcv-pc-role:" + _pc_role + "\r";
    }

    std::string SDPMungler::rcv_p2p() const
    {
        return _p2p ? "a=rcv-p2p\r": "";
    }

    std::string SDPMungler::rcv_local_party() const
    {
        return _local_party.empty() ? "" : "a=rcv-local-party:" + _local_party + "\r";
    }

    std::string SDPMungler::rcv_remote_party() const
    {
        return _remote_party.empty() ? "" : "a=rcv-remote-party:" + _remote_party + "\r";
    }

    std::string SDPMungler::rcv_remote_session() const
    {
        return _remote_session.empty() ? "" : "a=rcv-remote-session:" + _remote_session + "\r";
    }
    
    std::string SDPMungler::rcv_mix_audio() const {
        return _mix_audio_recv.empty() ? "" : "a=rcv-mix-audio:recv-mode=" + _mix_audio_recv + "\r";
    }
    
    std::string SDPMungler::rcv_negotiate_video() const {
        return _negotiate_video_recv.empty() ? "" : "a=rcv-negotiate-video:recv-mode=" + _negotiate_video_recv + "\r";
    }

    bool SDPMungler::p2p_sdp() const
    {
        return _local_party.size() && _remote_party.size();
    }

    std::string SDPMungler::rcv_denoise_at_sfu() const
    {
        std::string enabled = _denoise_at_sfu ? "1" : "0";
        return _denoise_at_sfu ? "" : ("a=rcv-denoise:enabled=" + enabled + "\r");
    }

    void SDPMungler::inject_rcv(std::vector<std::string> &lines) const
    {
        std::vector<std::string> rcv_attributes;
        push_back_not_empty(rcv_attributes, rcv_pc_role());
        push_back_not_empty(rcv_attributes, rcv_p2p());
        push_back_not_empty(rcv_attributes, rcv_remote_party());
        push_back_not_empty(rcv_attributes, rcv_local_party());
        push_back_not_empty(rcv_attributes, rcv_track());
        push_back_not_empty(rcv_attributes, rcv_remote_session());
        push_back_not_empty(rcv_attributes, rcv_mix_audio());
        push_back_not_empty(rcv_attributes, rcv_negotiate_video());
        push_back_not_empty(rcv_attributes, rcv_denoise_at_sfu());

        if (rcv_attributes.size())
        {
            const auto it = rcv_a_it(lines);
            if (it != lines.end())
            {
                lines.insert(it, rcv_attributes.begin(), rcv_attributes.end());
            }
        }
    }

    void SDPMungler::inject_simulcast_by_unified_plan(std::vector<std::string> & lines) const{
        cout << "inject begin" << endl;
        if (_simulcast < 1 || _simulcast > 2)
        {
        cout << "inject sim: return;" << endl;
            return;
        }

        std::string ssrc, ssrc_fid, cname, mslabel, label;
        size_t index_to_inject = 0;
        bool m_video = false;
        bool has_simulcast = false;
        bool isFirstMid = true;

        for (size_t i = 0; i < lines.size(); ++i)
        {
            const auto &line = lines[i];
            std::string mtype;
            if (parse_mline(line, mtype))
            {
                cout << "has mline" << endl;
                if (!m_video && mtype == "video")
                {
                    if(isFirstMid){
                        isFirstMid = false;
                cout << "fist mid is sharing, just continue" << endl;
                        continue;
                    }

                cout << "video is true" << endl;
                    m_video = true;
                    continue;
                }
                if (m_video)
                {
                    m_video = false;
                    index_to_inject = i;
                cout << "has video, set video false" << endl;
                cout << "index: " << index_to_inject << endl;
                    continue;
                }
            }
            if (!m_video)
            {
                cout << "has not video, continue" << endl;
                continue;
            }
            if (ssrc.empty())
            {
                cout << "ssrc is empty" << endl;
                if (parse_fid(line, ssrc, ssrc_fid))
                {
                    cout << "get fid" << endl;
                    continue;
                }
                parse_cname(line, ssrc, cname);
            }

            if (ssrc.length())
            {
                std::string tmp_ssrc, value;
                if (cname.empty() && parse_cname(line, tmp_ssrc, value) && tmp_ssrc == ssrc)
                {
                    cname = value;
                    continue;
                }

                if (mslabel.empty() && parse_mslabel(line, tmp_ssrc, value) && tmp_ssrc == ssrc)
                {
                    mslabel = value;
                    continue;
                }

                if (label.empty() && parse_label(line, tmp_ssrc, value) && tmp_ssrc == ssrc)
                {
                    label = value;
                    continue;
                }
            }

            if (line.find("a=ssrc-group:SIM ") == 0) {
                has_simulcast = true;
            }
        }

        cout << "parse end" << endl;
        cout << has_simulcast << ", " << cname << ", " << mslabel << ", " << label << endl;
        if (!has_simulcast && ssrc.length() && cname.length() && mslabel.length() && label.length())
        {
            std::stringstream group;
            std::vector<std::string> simulcast_lines;
            group << "a=ssrc-group:SIM " + ssrc;
            for (int64_t i = 0; i < _simulcast; ++i)
            {
                const auto e_ssrc = inject_ssrc_and_fid(simulcast_lines, cname, mslabel, label, ssrc_fid.length());
                group << " " << e_ssrc;
            }
            group << "\r";
            simulcast_lines.emplace_back(group.str());
            const auto insert_it = index_to_inject ? lines.begin() + index_to_inject : lines.end();
            lines.insert(insert_it, simulcast_lines.begin(), simulcast_lines.end());
        }

    }

    void SDPMungler::inject_simulcast(std::vector<std::string> &lines) const
    {
        cout << "inject begin" << endl;
        if (_simulcast < 1 || _simulcast > 2)
        {
        cout << "inject sim: return;" << endl;
            return;
        }

        std::string ssrc, ssrc_fid, cname, mslabel, label;
        size_t index_to_inject = 0;
        bool m_video = false;
        bool has_simulcast = false;

        for (size_t i = 0; i < lines.size(); ++i)
        {
            const auto &line = lines[i];
            std::string mtype;
            if (parse_mline(line, mtype))
            {
                cout << "has mline" << endl;
                if (!m_video && mtype == "video")
                {
                cout << "video is true" << endl;
                    m_video = true;
                    continue;
                }
                if (m_video)
                {
                    m_video = false;
                    index_to_inject = i - 1;
                cout << "has video, set video false" << endl;
                cout << "index: " << index_to_inject << endl;
                    continue;
                }
            }
            if (!m_video)
            {
                cout << "has not video, continue" << endl;
                continue;
            }
            if (ssrc.empty())
            {
                cout << "ssrc is empty" << endl;
                if (parse_fid(line, ssrc, ssrc_fid))
                {
                    cout << "get fid" << endl;
                    continue;
                }
                parse_cname(line, ssrc, cname);
            }

            if (ssrc.length())
            {
                std::string tmp_ssrc, value;
                if (cname.empty() && parse_cname(line, tmp_ssrc, value) && tmp_ssrc == ssrc)
                {
                    cname = value;
                    continue;
                }

                if (mslabel.empty() && parse_mslabel(line, tmp_ssrc, value) && tmp_ssrc == ssrc)
                {
                    mslabel = value;
                    continue;
                }

                if (label.empty() && parse_label(line, tmp_ssrc, value) && tmp_ssrc == ssrc)
                {
                    label = value;
                    continue;
                }
            }

            if (line.find("a=ssrc-group:SIM ") == 0) {
                has_simulcast = true;
            }
        }

        cout << "parse end" << endl;
        cout << has_simulcast << ", " << cname << ", " << mslabel << ", " << label << endl;
        if (!has_simulcast && ssrc.length() && cname.length() && mslabel.length() && label.length())
        {
            std::stringstream group;
            std::vector<std::string> simulcast_lines;
            group << "a=ssrc-group:SIM " + ssrc;
            for (int64_t i = 0; i < _simulcast; ++i)
            {
                const auto e_ssrc = inject_ssrc_and_fid(simulcast_lines, cname, mslabel, label, ssrc_fid.length());
                group << " " << e_ssrc;
            }
            group << "\r";
            simulcast_lines.emplace_back(group.str());
            const auto insert_it = index_to_inject ? lines.begin() + index_to_inject : lines.end();
            lines.insert(insert_it, simulcast_lines.begin(), simulcast_lines.end());
        }
    }

    void SDPMungler::inject_conference(std::vector<std::string> &lines) const
    {
        if (_inject_conference)
        {
            lines.emplace_back("a=x-google-flag:conference\r");
        }
    }


    
    std::vector<std::string> SDPMungler::minify(const std::vector<std::string> &lines) const {
        std::vector<std::string> result;
        result.reserve(6);
        result.push_back(lines[0]);
        result.push_back(lines[1]);
        result.emplace_back("a=rcv-minified\r");
        push_back_not_empty(result, rcv_pc_role());
        push_back_not_empty(result, rcv_remote_session());
        result.emplace_back("");
        return result;
    }









int main()
{

    // SDPMungler sdpMungler(aOffer);
    SDPMungler sdpMungler(bOffer);
    sdpMungler.inject_simulcast(2);
    cout << sdpMungler.result() << endl;

    return 0;
}