//////////////////////////////////////////////////////////////////////////
//
//	Crytek Source code
//	Copyright (c) Crytek 2001-2004
//
//	File: Cryversion.h
//	Description: Defines File version structure.
//
//	History:
//	- 27/8/2003: Created by Timur Davidenko
//	- February 2005: Modified by Marco Corbetta for SDK release
//
//////////////////////////////////////////////////////////////////////

#ifndef __cryversion_h__
#define __cryversion_h__

#include <cstring>
#include <cstdlib>
#include <cstdio>

//////////////////////////////////////////////////////////////////////////
/** This class keeps file version information.
*/
struct SFileVersion
{
  int v[4];

  SFileVersion() {
    v[0] = v[1] = v[2] = v[3] = 0;
  }
  SFileVersion(const int vers[]) {
    v[0] = vers[0];
    v[1] = vers[1];
    v[2] = vers[2];
    v[3] = 1;
  }

  void	Set(const char* s)
  {
    v[0] = v[1] = v[2] = v[3] = 0;

    char t[50];	char* p;
    strcpy(t, s);

    if (!(p = strtok(t, "."))) return;
    v[3] = atoi(p);
    if (!(p = strtok(NULL, "."))) return;
    v[2] = atoi(p);
    if (!(p = strtok(NULL, "."))) return;
    v[1] = atoi(p);
    if (!(p = strtok(NULL, "."))) return;
    v[0] = atoi(p);
  }

  explicit SFileVersion(const char* s)
  {
    Set(s);
  }

  bool operator <(const SFileVersion& v2) const {
    if (v[3] < v2.v[3]) return true;
    if (v[3] > v2.v[3]) return false;

    if (v[2] < v2.v[2]) return true;
    if (v[2] > v2.v[2]) return false;

    if (v[1] < v2.v[1]) return true;
    if (v[1] > v2.v[1]) return false;

    if (v[0] < v2.v[0]) return true;
    if (v[0] > v2.v[0]) return false;
    return false;
  }
  bool operator ==(const SFileVersion& v1) const {
    if (v[0] == v1.v[0] && v[1] == v1.v[1] &&
      v[2] == v1.v[2] && v[3] == v1.v[3]) return true;
    return false;
  }
  bool operator >(const SFileVersion& v1) const {
    return !(*this < v1);
  }
  bool operator >=(const SFileVersion& v1) const {
    return (*this == v1) || (*this > v1);
  }
  bool operator <=(const SFileVersion& v1) const {
    return (*this == v1) || (*this < v1);
  }

  int& operator[](int i) { return v[i]; }
  int  operator[](int i) const { return v[i]; }

  void ToShortString(char* s) const
  {
    sprintf(s, "%d.%d.%d", v[2], v[1], v[0]);
  }

  void ToString(char* s) const
  {
    sprintf(s, "%d.%d.%d.%d", v[3], v[2], v[1], v[0]);
  }
};

#endif // __cryversion_h__