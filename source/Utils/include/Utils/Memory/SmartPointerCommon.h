#ifndef SMART_POINTER_COMMON_H
#define SMART_POINTER_COMMON_H

#define COMMA ,
#define DECLARE_SMART_POINTER_COMPARAISON_OPERATORS(templateDeclaration, type) \
    templateDeclaration \
    inline bool operator==(const type& left, const type& right) \
    { \
        return left.get() == right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator!=(const type& left, const type& right) \
    { \
        return left.get() != right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator<(const type& left, const type& right) \
    { \
        return left.get() < right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator<=(const type& left, const type& right) \
    { \
        return left.get() <= right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator>(const type& left, const type& right) \
    { \
        return left.get() > right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator>=(const type& left, const type& right) \
    { \
        return left.get() >= right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator==(const type& left, typename type::PointerType right) \
    { \
        return left.get() == right; \
    } \
    \
    templateDeclaration \
    inline bool operator!=(const type& left, typename type::PointerType right) \
    { \
        return left.get() != right; \
    } \
    \
    templateDeclaration \
    inline bool operator<(const type& left, typename type::PointerType right) \
    { \
        return left.get() < right; \
    } \
    \
    templateDeclaration \
    inline bool operator<=(const type& left, typename type::PointerType right) \
    { \
        return left.get() <= right; \
    } \
    \
    templateDeclaration \
    inline bool operator>(const type& left, typename type::PointerType right) \
    { \
        return left.get() > right; \
    } \
    \
    templateDeclaration \
    inline bool operator>=(const type& left, typename type::PointerType right) \
    { \
        return left.get() >= right; \
    } \
    \
    templateDeclaration \
    inline bool operator==(typename type::PointerType left, const type& right) \
    { \
        return left == right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator!=(typename type::PointerType left, const type& right) \
    { \
        return left != right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator<(typename type::PointerType left, const type& right) \
    { \
        return left < right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator<=(typename type::PointerType left, const type& right) \
    { \
        return left <= right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator>(typename type::PointerType left, const type& right) \
    { \
        return left > right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator>=(typename type::PointerType left, const type& right) \
    { \
        return left >= right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator==(const type& left, std::nullptr_t right) \
    { \
        return left.get() == right; \
    } \
    \
    templateDeclaration \
    inline bool operator!=(const type& left, std::nullptr_t right) \
    { \
        return left.get() != right; \
    } \
    \
    templateDeclaration \
    inline bool operator<(const type& left, std::nullptr_t right) \
    { \
        return left.get() < right; \
    } \
    \
    templateDeclaration \
    inline bool operator<=(const type& left, std::nullptr_t right) \
    { \
        return left.get() <= right; \
    } \
    \
    templateDeclaration \
    inline bool operator>(const type& left, std::nullptr_t right) \
    { \
        return left.get() > right; \
    } \
    \
    templateDeclaration \
    inline bool operator>=(const type& left, std::nullptr_t right) \
    { \
        return left.get() >= right; \
    } \
    \
    templateDeclaration \
    inline bool operator==(std::nullptr_t left, const type& right) \
    { \
        return left == right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator!=(std::nullptr_t left, const type& right) \
    { \
        return left != right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator<(std::nullptr_t left, const type& right) \
    { \
        return left < right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator<=(std::nullptr_t left, const type& right) \
    { \
        return left <= right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator>(std::nullptr_t left, const type& right) \
    { \
        return left > right.get(); \
    } \
    \
    templateDeclaration \
    inline bool operator>=(std::nullptr_t left, const type& right) \
    { \
        return left >= right.get(); \
    }

#endif