#pragma once

#include <string>
#include <cstring>
#include <cctype>

namespace FileExt {
  inline bool hasExt(std::string_view s,const char* extIn) {
    if(extIn==nullptr)
      return std::string::npos==s.find('.');
    const size_t l = std::strlen(extIn);
    if(l+1>s.size())
      return false;
    const size_t off = s.size()-l;
    if(s[off-1]!='.')
      return false;
    for(size_t i=0;i<l;++i) {
      char a = char(std::tolower(s[off+i]));
      char b = char(std::tolower(extIn[i]));
      if(a!=b)
        return false;
      }
    return true;
    }

  inline bool hasExt(const char* s,const char* extIn) {
    const size_t ssize = std::strlen(s);
    const size_t l     = std::strlen(extIn);
    if(l+1>ssize)
      return false;
    const size_t off = ssize-l;
    if(s[off-1]!='.')
      return false;
    for(size_t i=0;i<l;++i) {
      char a = char(std::tolower(s[off+i]));
      char b = char(std::tolower(extIn[i]));
      if(a!=b)
        return false;
      }
    return true;
    }

  inline bool exchangeExt(std::string& s,const char* extIn,const char* extOut) {
    if(!hasExt(s,extIn))
      return false;
    if(extIn==nullptr) {
      s.push_back('.');
      s += extOut;
      return true;
      }
    const size_t l1 = std::strlen(extIn);
    const size_t l2 = std::strlen(extOut);
    if(l1<l2)
      s.resize(s.size()+l2-l1); else
    if(l1>l2)
      s.resize(s.size()+l1-l2);

    const size_t off = s.size()-l2;
    for(size_t i=0;i<l2;++i)
      s[off+i] = extOut[i];
    return true;
    }

  inline void assignExt(std::string& s,const char* extOut){
    size_t cut = s.rfind('.');
    if(cut==std::string::npos) {
      s.push_back('.');
      s += extOut;
      return;
      }
    const size_t l1 = s.size()-cut-1;
    const size_t l2 = std::strlen(extOut);
    if(l1<l2)
      s.resize(s.size()+l2-l1); else
    if(l1>l2)
      s.resize(s.size()+l1-l2);
    const size_t off = cut+1;
    for(size_t i=0;i<l2;++i)
      s[off+i] = extOut[i];
    }

  inline std::string addExt(const std::string& s,const char* ext){
    if(s.size()>0 && s.back()=='.')
      return s+&ext[1];
    return s+ext;
    }

  inline std::string removeExt(const std::string& s) {
    size_t cut = s.rfind('.');
    if(cut==std::string::npos)
      return s;
    return s.substr(0, cut);
    }
  }
