/// A Shared String class

#pragma once

#include <string>
#include <string_view>
#include <memory>

namespace jsonlogic
{
  using shared_string_ptr = std::shared_ptr<std::string>;

  struct managed_string_view : private shared_string_ptr, public std::string_view
  {
      struct no_lifetime_management {};

      static
      std::string_view to_string_view(shared_string_ptr p) { return *p; }

      using holder    = shared_string_ptr;
      using base      = std::string_view;
      using size_type = std::string_view::size_type;

      ~managed_string_view()                                     = default;
      managed_string_view(const managed_string_view&)            = default;
      managed_string_view(managed_string_view&&)                 = default;
      managed_string_view& operator=(const managed_string_view&) = default;
      managed_string_view& operator=(managed_string_view&&)      = default;

      /// constructs a managed_string_view with a fresh shared string
      explicit
      managed_string_view(std::string_view view)
      : holder(std::make_shared<std::string>(view.begin(), view.end())), base(to_string_view(*this))
      {}

      /// constructs a managed_string_view object without managing the
      /// life time of an underlying string.
      /// \details
      ///   In this case the caller needs to guarantee that the view's
      ///   underlying string outlives the view.
      ///   One use case are string constants in jsonlogic, such as "true", "null"
      managed_string_view(std::string_view view, no_lifetime_management)
      : holder(nullptr), base(view)
      {}

      /// constructs a managed_string_view from a shared string.
      /// \param string_ptr the shared string pointer
      managed_string_view(shared_string_ptr string_ptr)
      : holder(std::move(string_ptr)), base()
      {
        holder& this_string_ptr = static_cast<holder&>(*this);

        if (this_string_ptr != nullptr)
          static_cast<base&>(*this) = std::string_view(*this_string_ptr);
      }

      /// creates a new managed_string_view from \p s.
      // implicit
      managed_string_view(std::string&& s)
      : holder(std::make_shared<std::string>(std::move(s))), base(to_string_view(*this))
      {}

    private:
      managed_string_view(holder string_ptr, std::string_view view)
      : holder(std::move(string_ptr)), base(view)
      {}

    public:
      managed_string_view substr(size_type ofs = 0, size_type cnt = base::npos) const
      {
        return { holder(*this), base::substr(ofs, cnt) };
      }

      managed_string_view substr(const char* beg, std::size_t cnt = base::npos) const
      {
        return substr(std::distance(&this->at(0), beg), cnt);
      }

      std::string_view view() const { return *this; }
  };

  // \todo replace with space ship operator
  inline bool operator==(const managed_string_view& lhs, const managed_string_view& rhs) { return lhs.view() == rhs.view(); }
  inline bool operator!=(const managed_string_view& lhs, const managed_string_view& rhs) { return lhs.view() != rhs.view(); }
  inline bool operator<(const managed_string_view& lhs, const managed_string_view& rhs)  { return lhs.view() < rhs.view(); }
  inline bool operator<=(const managed_string_view& lhs, const managed_string_view& rhs) { return lhs.view() <= rhs.view(); }
  inline bool operator>(const managed_string_view& lhs, const managed_string_view& rhs)  { return lhs.view() > rhs.view(); }
  inline bool operator>=(const managed_string_view& lhs, const managed_string_view& rhs) { return lhs.view() >= rhs.view(); }
}
