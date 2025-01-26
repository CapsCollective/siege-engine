//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Token.h"

#include "Logging.h"

namespace Siege
{
Token::Token() : tokenId(nullptr) {}

Token::Token(const char* name) : tokenId(FindTokenId(name)) {}

bool Token::operator==(Token rhs) const
{
    return tokenId == rhs.tokenId;
}

bool Token::operator!=(Token rhs) const
{
    return tokenId != rhs.tokenId;
}

bool Token::operator<(Token rhs) const
{
    return tokenId < rhs.tokenId;
}

const char* Token::GetId() const
{
    return tokenId;
}

bool Token::IsValid() const
{
    return tokenId != nullptr;
}

std::unordered_set<String>& Token::GetGlobalTokenRegister()
{
    static std::unordered_set<String> tokenRegister;
    return tokenRegister;
}

Token Token::FindToken(const String& name)
{
    return Token(FindTokenId(name));
}

Token Token::FindOrRegisterToken(const String& name)
{
    if (name.IsEmpty()) return {};

    std::unordered_set<String>& tokenRegister = GetGlobalTokenRegister();
    auto it = tokenRegister.insert(name);
    if (it.second) CC_LOG_INFO("Registered new token \"{}\"", name);

    return Token(it.first->Str());
}

const char* Token::FindTokenId(const String& name)
{
    const std::unordered_set<String>& tokenRegister = GetGlobalTokenRegister();
    auto it = tokenRegister.find(name);

    return it != tokenRegister.end() ? it->Str() : nullptr;
}

TokenRegisterer::TokenRegisterer(const char* name)
{
    Token::FindOrRegisterToken(name);
}
} // namespace Siege