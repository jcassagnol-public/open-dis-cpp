// TODO add copyright aggregate here

#ifndef _dcl_dis_base64_processor_h_
#define _dcl_dis_base64_processor_h_

#include <string>
#if __cplusplus >= 201703L
#include <string_view>
#endif
#include <type_traits>
#include <boost/algorithm/hex.hpp>

namespace DIS
{
    namespace impl
    {
        /**
         * @brief Converts an arbitrary char array to a hex string.
         * 
         * @param buf the buf to convert to a hex string array.
         * @param len the length of buf.
         * @return std::string the converted hex string.
         */
        inline std::string hexEncode(const char* buf, size_t len)
        {
#if __cplusplus >= 201703L
            std::string_view tempStrView{buf, len};
            return boost::algorithm::hex(tempStrView)
#else
            std::string tempStr{buf, len};
            return boost::algorithm::hex(tempStr);
#endif
        }

        /**
         * @brief Converts an arbitrary hex array to a unhexed string.
         * 
         * @param buf the buf to convert from a hex string array to an unhexed string.
         * @param len the length of buf.
         * @return std::string the converted un-hexed string.
         */
        inline std::string hexDecode(const char* buf, size_t len)
        {
#if __cplusplus >= 201703L
            std::string_view tempStrView{buf, len};
            return boost::algorithm::unhex(tempStrView)
#else
            std::string tempStr{buf, len};
            return boost::algorithm::unhex(tempStr);
#endif
        }
    }

    template <size_t array_len>
    std::string toHex(const char (&in)[array_len])
    {
        return impl::hexEncode(in, array_len);
    }

    inline std::string toHex(std::vector<char> in)
    {
        return impl::hexEncode(in.data(), in.size());
    }

    template <size_t array_len>
    void fromHex(const std::string &in, char (&out)[array_len])
    {
        std::string temp = impl::hexDecode(in.data(), in.size());
        if (array_len < temp.size())
        {
            throw std::runtime_error("Bad base64 data decode, " + temp + " has length > max size of output array, " + std::to_string(array_len));
        }
        else
        {
            std::copy(temp.begin(), temp.end(), out);
        }
    }

    inline void fromHex(const std::string &in, std::vector<char> &out)
    {
        std::string temp = impl::hexDecode(in.data(), in.size());
        out.clear();
        std::copy(temp.begin(), temp.end(), std::back_inserter(out));
    }
} // namespace DIS

#endif // _dcl_dis_base64_processor_h_
