#ifndef STRING_UTILS_HPP_
#define STRING_UTILS_HPP_

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <variant>

namespace strutil {
constexpr bool starts_with(std::string_view text, std::string_view prefix)
{
    return (text.size() >= prefix.size()) &&
           (text.substr(0, prefix.size()) == prefix);
}

constexpr bool ends_with(std::string_view text, std::string_view suffix)
{
   return (text.size() >= suffix.size()) &&
          (text.substr(text.size() - suffix.size()) == suffix); 
}

inline std::string upper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
    return str;
}

inline std::string lower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return str;
}

inline std::string capitalize(std::string str)
{
    if (str.size() != 0)
        str[0] = std::toupper(str[0]);
    return str;
}

inline std::string lstrip(std::string_view text,
                          std::string_view prefix)
{
    if(starts_with(text, prefix))
        text.remove_prefix(prefix.size());
    return std::string(text);
}

inline std::string rstrip(std::string_view text,
                          std::string_view suffix)
{
    if(ends_with(text, suffix))
        text.remove_suffix(suffix.size());
    return std::string(text);
}

inline std::string strip(std::string_view text,
                         std::string_view  ends)
{
    return rstrip(lstrip(text, ends), ends);
}

inline std::string lstrip_any_of(std::string_view text,
                                 std::string_view chars = " \n\r\t")
{
    auto const pos = text.find_first_not_of(chars);
    text.remove_prefix(std::min(text.size(), pos));
    return std::string(text);
}

inline std::string rstrip_any_of(std::string_view text,
                                 std::string_view chars = " \n\r\t")
{
    auto const pos = text.find_last_not_of(chars);
    text.remove_suffix(std::min(text.size(), text.size() - pos - 1));
    return std::string(text);
}

inline std::string strip_any_of(std::string_view text,
                                std::string_view chars = " \n\r\t")
{
    return rstrip_any_of(lstrip_any_of(text, chars), chars);
}

inline void replace(std::string& str,
                    std::string_view old,
                    std::string_view new_)
{
    size_t pos = 0;
    while ((pos = str.find_first_of(old)) != std::string::npos) {
        str.replace(pos, old.size(), new_);
        pos += old.size();
    }
}

inline std::string replace_copy(std::string_view str,
                                std::string_view old,
                                std::string_view new_)
{
    std::string result(str);
    replace(result, old, new_);
    return result;
}

namespace internal {
template<typename Split>
struct split_iterator
{
    using Self              = split_iterator<Split>;
    using iterator_category = std::input_iterator_tag;
    using value_type        = typename Split::value_type;
    using reference         = value_type const&;
    using pointer           = value_type const*;
    using difference_type   = ptrdiff_t;

  public:
    split_iterator() noexcept
      : split_{}
      , view_{}
    {}

    explicit split_iterator(Split const* split) noexcept
      : split_(split)
      , view_(split_->value().data(),
              std::min(split_->value().size(),
                       split_->value().find(split_->delimiter())))
    {}

    pointer operator->() const noexcept
    {
        return &view_;
    }

    reference operator*() const noexcept
    {
        return view_;
    }

    Self& operator++() noexcept
    {
        move_view_forward_();
        return *this;
    }

    Self operator++(int) noexcept
    {
        Self old = *this;
        ++*this;
        return old;
    }

    bool operator==(Self const&) const noexcept
    {
        return view_.data() == nullptr;
    }

    bool operator!=(Self const&) const noexcept
    {
        return view_.data() != nullptr;
    }
    
  private:
    Split const* split_;
    value_type view_;

  private:
    void move_view_forward_() noexcept
    {
        if (view_.data() == nullptr)
            return;
        using size_type = typename value_type::size_type;
        auto const& text = split_->value();
        auto const& delim = split_->delimiter();
        size_type const delim_size = std::max(size_type(1), delim.size());
        auto const pos_ = (view_.data() - text.data()) + view_.size() + delim_size;
        if (pos_ <= text.size())
        {
            auto const tpos = std::min(text.find(delim, pos_), text.size());
            view_ = text.substr(pos_, tpos - pos_);
        }
        else
        {
            view_ = value_type(nullptr, 0);
        }
    }
};
} // namespace internal

template<typename CharT,
         typename Traits = std::char_traits<CharT>,
         typename Alloc = std::allocator<CharT>>
struct split
{
    using iterator       = internal::split_iterator<split<CharT, Traits, Alloc>>;
    using const_iterator = iterator;
    using value_type     = std::basic_string_view<CharT, Traits>;

  private:
    using bstring = std::basic_string<CharT, Traits, Alloc>;

    struct Visitor
    {
        value_type operator()(bstring const& s)
        {
            return { s.c_str(), s.size() };
        }

        value_type operator()(value_type s)
        {
            return s;
        }
    };

  public:
    split(CharT const* input, std::basic_string_view<CharT, Traits> delim)
      : view_(value_type(input))
      , delim_(delim)
    {}

    split(CharT* input, value_type delim)
      : view_(value_type(input))
      , delim_(delim)
    {}

    split(std::basic_string<CharT, Traits, Alloc> const& input,
          std::basic_string_view<CharT, Traits> delim)
      : view_(value_type(input.c_str(), input.size()))
      , delim_(delim)
    {}

    split(std::basic_string<CharT, Traits, Alloc>&& input,
          std::basic_string_view<CharT, Traits> delim)
      : view_(std::move(input))
      , delim_(delim)
    {}

    split(std::basic_string_view<CharT, Traits> input,
          std::basic_string_view<CharT, Traits> delim)
      : view_(std::move(input))
      , delim_(delim)
    {}

    value_type value() const noexcept
    {
        return std::visit(Visitor{}, view_);
    }

    value_type delimiter() const noexcept
    {
        return value_type(delim_.c_str(), delim_.size());
    }

    iterator begin() const noexcept
    {
        return iterator(this);
    }

    iterator end() const noexcept
    {
        return iterator();
    }

    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    const_iterator cend() const noexcept
    {
        return end();
    }

  private:
    std::variant<bstring, value_type> view_;
    std::basic_string<CharT, Traits, Alloc> delim_;
};

#if __cpp_deduction_guides >= 201606
template<typename Str>
split(Str, typename Str::value_type const*)
  -> split<typename Str::value_type,
           typename Str::traits_type,
           typename Str::allocator_type>;

template<typename CharT>
split(CharT const*, CharT const*) -> split<CharT>;
#endif

} // end namespace strutil

#endif
