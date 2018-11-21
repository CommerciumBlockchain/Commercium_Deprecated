// Copyright (c) 2009-2016 The Commercium developers
class CCommerciumAddressVisitor : public boost::static_visitor<bool> {
private:
    CCommerciumAddress *addr;

public:
    CCommerciumAddressVisitor(CCommerciumAddress *addrIn) : addr(addrIn) {}

    bool operator()(const CKeyID &id) const { return addr->Set(id); }
    bool operator()(const CScriptID &id) const { return addr->Set(id); }
    bool operator()(const CNoDestination &no) const { return false; }
};

} // anon namespace

bool CCommerciumAddress::Set(const CKeyID &id) {
    SetData(Params().Base58Prefix(CChainParams::PUBKEY_ADDRESS), &id, 20);
    return true;
}

bool CCommerciumAddress::Set(const CScriptID &id) {
    SetData(Params().Base58Prefix(CChainParams::SCRIPT_ADDRESS), &id, 20);
    return true;
}

bool CCommerciumAddress::Set(const CTxDestination &dest) {
    return boost::apply_visitor(CCommerciumAddressVisitor(this), dest);
}

bool CCommerciumAddress::IsValid() const {
    return IsValid(Params());
}

bool CCommerciumAddress::IsValid(const CChainParams &params) const {
    bool fCorrectSize = vchData.size() == 20;
    bool fKnownVersion =
        vchVersion == params.Base58Prefix(CChainParams::PUBKEY_ADDRESS) ||
        vchVersion == params.Base58Prefix(CChainParams::SCRIPT_ADDRESS);
    return fCorrectSize && fKnownVersion;
}

CTxDestination CCommerciumAddress::Get() const {
    if (!IsValid()) return CNoDestination();
    uint160 id;
    memcpy(&id, &vchData[0], 20);
    if (vchVersion == Params().Base58Prefix(CChainParams::PUBKEY_ADDRESS)) {
        return CKeyID(id);
    } else if (vchVersion ==
               Params().Base58Prefix(CChainParams::SCRIPT_ADDRESS)) {
        return CScriptID(id);
    } else {
        return CNoDestination();
    }
}

bool CCommerciumAddress::GetIndexKey(uint160& hashBytes, int& type) const
{
    if (!IsValid()) {
        return false;
    } else if (vchVersion == Params().Base58Prefix(CChainParams::PUBKEY_ADDRESS)) {
        memcpy(&hashBytes, &vchData[0], 20);
        type = 1;
        return true;
    } else if (vchVersion == Params().Base58Prefix(CChainParams::SCRIPT_ADDRESS)) {
        memcpy(&hashBytes, &vchData[0], 20);
        type = 2;
        return true;
    }

    return false;
}

void CCommerciumSecret::SetKey(const CKey &vchSecret) {
    assert(vchSecret.IsValid());
    SetData(Params().Base58Prefix(CChainParams::SECRET_KEY), vchSecret.begin(),
            vchSecret.size());
    if (vchSecret.IsCompressed()) vchData.push_back(1);
}

CKey CCommerciumSecret::GetKey() {
    CKey ret;
    assert(vchData.size() >= 32);
    ret.Set(vchData.begin(), vchData.begin() + 32,
            vchData.size() > 32 && vchData[32] == 1);
    return ret;
}

bool CCommerciumSecret::IsValid() const {
    bool fExpectedFormat =
        vchData.size() == 32 || (vchData.size() == 33 && vchData[32] == 1);
    bool fCorrectVersion =
        vchVersion == Params().Base58Prefix(CChainParams::SECRET_KEY);
    return fExpectedFormat && fCorrectVersion;
}

bool CCommerciumSecret::SetString(const char *pszSecret) {
    return CBase58Data::SetString(pszSecret) && IsValid();
}

bool CCommerciumSecret::SetString(const std::string &strSecret) {
    return SetString(strSecret.c_str());
}

std::string EncodeDestination(const CTxDestination &dest) {
    CCommerciumAddress addr(dest);
    return addr.IsValid() ? addr.ToString() : "";
}

CTxDestination DecodeDestination(const std::string &str) {
    return CCommerciumAddress(str).Get();
}

bool IsValidDestinationString(const std::string &str,
                              const CChainParams &params) {
    return CCommerciumAddress(str).IsValid(params);
}

bool IsValidDestinationString(const std::string &str) {
    return CCommerciumAddress(str).IsValid();
}
