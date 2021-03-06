/*
 *      Copyright (C) 2015 Anthony Miyaguchi
 *      Copyright (C) 2015 Team XBMC
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <string>
#include <vector>
#include <array>
#include <openssl/x509.h>
#include <openssl/evp.h>

namespace MOONLIGHT
{
  enum class PairState
  {
    NOT_PAIRED, PAIRED, PIN_WRONG, FAILED
  };

  class NvHTTP;
  class CertKeyPair;

  class PairingManager
  {
  public:
    PairingManager(NvHTTP* http, CertKeyPair* cert);
    PairState pair(std::string uid, std::string pin);
    PairState getPairState(std::string serverInfo);
    static std::string generatePinString();
    std::string bytesToHex(unsigned char* in, unsigned len);
    private:
    std::vector<unsigned char> hexToBytes(std::string s);
    bool verifySignature(std::vector<unsigned char> data, std::vector<unsigned char> signature, EVP_PKEY *pkey);
    std::vector<unsigned char> signData(std::vector<unsigned char> data, EVP_PKEY *pkey);

    NvHTTP* m_http;
    CertKeyPair* m_cert;
  };
}
