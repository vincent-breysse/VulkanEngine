#pragma once

#include <gyn/common/pch.hpp>

namespace Gyn
{
    struct FmtFormatter
    {
        static constexpr const char DefaultFormatStr[] = "{}";
        static constexpr const size_t DefaultFormatStrLen = sizeof(DefaultFormatStr) - 1; // Exclude trailing \0
        static constexpr fmt::string_view DefaultFormat = fmt::string_view(DefaultFormatStr, DefaultFormatStrLen);

        constexpr auto parse(fmt::format_parse_context& ctx)
        {
            m_format = DefaultFormat;

            auto begin = ctx.begin();
            auto end = ctx.end();

            if (!begin || !end)
            {
                return begin;
            }

            auto fmtBegin = begin;
            auto fmtEnd = begin;

            while (*fmtBegin != '{')
            {
                --fmtBegin;
            }

            while (fmtEnd != end && *fmtEnd != '}')
            {
                ++fmtEnd;
            }

            if (fmtEnd == end)
            {
                throw fmt::format_error("Invalid format");
            }

            m_format = fmt::string_view(fmtBegin, fmtEnd + 1 - fmtBegin); // Add 1 to include the ending '}'

            return fmtEnd;
        }

        fmt::string_view get_parsed_format() const
        {
            return m_format;
        }

    private:

        fmt::string_view m_format{};
    };
}

template <>
struct fmt::formatter<Gyn::float3> : public Gyn::FmtFormatter
{
    template <typename FormatContext>
    auto format(const Gyn::float3& v, FormatContext& ctx)
    {
        const auto format = fmt::format("({0}, {0}, {0})", get_parsed_format()); // Generate something like "({:.1f}, {:.1f}, {:.1f})"

        return format_to(ctx.out(), format, v.x, v.y, v.z);
    }
};