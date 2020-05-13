#version 330 core

/*
 * Original shader from: https://www.shadertoy.com/view/3slBRf
 */

#ifdef GL_ES
precision mediump float;
#endif

// glslsandbox uniforms
uniform float time;
uniform vec2 resolution;
uniform vec2 origin;

// shadertoy emulation
#define iTime time
#define iResolution resolution
const vec4 iMouse = vec4(0.);

// --------[ Original ShaderToy begins here ]---------- //
// "Sand Album" by dr2 - 2020
// License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License

float PrBoxDf (vec3 p, vec3 b);
float PrRoundBoxDf (vec3 p, vec3 b, float r);
float PrRoundBox2Df (vec2 p, vec2 b, float r);
float PrSphDf (vec3 p, float r);
float PrCylDf (vec3 p, float r, float h);
float PrCylAnDf (vec3 p, float r, float w, float h);
float PrRoundCylDf (vec3 p, float r, float rt, float h);
float PrCapsDf (vec3 p, float r, float h);
float PrTorusDf (vec3 p, float ri, float rc);
vec3 IcoSym (vec3 p);
vec3 DodSym (vec3 p);
mat3 StdVuMat (float el, float az);
vec2 Rot2D (vec2 q, float a);
vec2 Rot2Cs (vec2 q, vec2 cs);
float SmoothMin (float a, float b, float r);
float SmoothMax (float a, float b, float r);
float SmoothBump (float lo, float hi, float w, float x);
float Fbm1 (float p);
float Fbm2 (vec2 p);
float Fbm3 (vec3 p);
vec3 VaryNf (vec3 p, vec3 n, float f);

vec3 ltDirEx = vec3(0.), sunDir = vec3(0.), qHit = vec3(0.), pgSize = vec3(0.);
float tCur = 0., msAz = 0., msEl = 0., dstFarEx = 0., tpBook = 0., phsTurn = 0., idPage = 0., dstFar = 0., pileHt = 0., grRad = 0., pR = 0., pA = 0.,
   nStr = 0., pgRnd = 0., rc = 0.;
int idObj = 0;
const float pi = 3.14159, sqrt2 = 1.41421;

#define VAR_ZERO 0

#define DMIN(id) if (d < dMin) { dMin = d;  idObj = id; }
#define DMINQ(id) if (d < dMin) { dMin = d;  idObj = id;  qHit = q; }

float PileDf (vec3 p, float dMin)
{
  vec3 q;
  vec2 st;
  float d, r, s, a;
  q = p;
  q.y -= pileHt;
  s = q.y / pileHt;
  a = (nStr == 1. || mod (nStr, 2.) == 0.) ? pA : mod (pA + 0.5 / nStr, 1.);
  r = Fbm2 (vec2 (pgRnd + 64. * a, 1.5 * s));
  if (nStr == 0.) r = mix (r, Fbm2 (vec2 (pgRnd + 64. * (a - 1.), 1.5 * s)), a);
  r = 0.8 * r + 8. * (1. + 0.05 * (s - 1.5) * (s - 1.5));
  d = SmoothMax (pR - r, q.y - pileHt - 0.01, 0.2);
  if (nStr > 1.) q.xz = Rot2D (q.xz, 2. * pi * (floor (nStr * (pA - 0.5) + 0.5) / nStr));
  else q.xz *= -1.;
  st = q.xy - vec2 (-9., -0.05);
  if (nStr > 0.) d = max (d, min (0.6 - abs (q.z), (st.y - st.x + 0.2 -
     abs (mod (st.x + st.y, 0.4) - 0.2) / sqrt2)));
  d = SmoothMin (d, q.y + pileHt, 0.3);
  d = max (d, max (pR - grRad, - p.y));
  DMIN (2);  
  return 0.6 * dMin;
}

float CasBldgDf (vec3 p, float dMin)
{
  vec3 q, qq;
  float d, a;
  q = p;
  q.xz = Rot2D (q.xz, 2. * pi * (floor (4. * (pA - 0.5) + 0.5) / 4.));
  qq = q;
  q.xy -= vec2 (-5., 1.2);
  d = PrRoundBoxDf (q, vec3 (0.15, 1.2 + rc, 5.) - rc, rc);
  d = max (d, - PrRoundBox2Df (vec2 (q.y - 1.2, mod (q.z + 0.25, 0.5) - 0.25), vec2 (0.2, 0.1), rc));
  d = max (d, - PrRoundBox2Df (vec2 (q.yz - vec2 (-0.6, 0.)), vec2 (0.6, 0.4), rc));
  dMin = min (dMin, d);
  q.z = abs (q.z) - 5.;
  q.y -= 0.2;
  d = min (PrCylAnDf (q.xzy, 0.45, 0.05, 1.2 + 0.2), PrCylDf (q.xzy, 0.4, 1.2));
  dMin = min (dMin, d);
  q = qq;
  q.xy -= vec2 (-2., 2.);
  d = max (max (q.y - 1.8 - 0.3 * q.x, - q.x), - p.y);
  dMin = min (dMin, d);
  d = PrRoundBoxDf (q, vec3 (0.15, 2.2 + rc, 2.) - rc, rc);
  d = max (d, - PrRoundBox2Df (vec2 (q.y - 2.2, mod (q.z + 0.25, 0.5) - 0.25), vec2 (0.2, 0.1), rc));
  dMin = min (dMin, d);
  q.z = abs (q.z) - 2.;
  d = PrCylDf (q.xzy, 0.3, 2.3);
  dMin = min (dMin, d);
  nStr = 4.;
  return dMin;
}

float SprBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d, h;
  q = p;
  q.y -= 2.5;
  d = max (PrRoundBox2Df (vec2 (pR - (4. - 0.35 * q.y),
     mod (q.y + 1. + 2. * pA, 2.) - 1.), vec2 (0.4, 0.5), rc), abs (q.y + 0.5) - 3.);
  d = min (d, PrCylDf (q.xzy, 4. - 0.2 * q.y, 2.5));
  dMin = min (dMin, d);
  h = 1.3;
  q.y -= 2.5 + h;
  d = PrCylDf (q.xzy, 0.5 * (1. + 0.2 * sin (2. * pi * (2. * q.y / h + pA))) * (1. - 0.95 * q.y / h), h);
  dMin = min (dMin, d);
  nStr = 0.;
  return 0.9 * dMin;
}

float TwrBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d;
  q = p;
  q.y -= 3.5 - rc;
  d = PrRoundBoxDf (q, vec3 (vec2 (1.3) * (1. - 0.05 * q.y), 3.5).xzy - rc, rc);
  q.xz = Rot2D (q.xz, 2. * pi * (floor (4. * (pA - 0.5) + 0.5) / 4.));
  q.xy -= vec2 (-4., -1.);
  d = min (d, PrRoundCylDf (q.xzy, 0.8 * (1. - 0.05 * q.y), rc, 2.5));
  q.x -= 1.7;
  d = min (d, PrRoundBoxDf (vec3 (q.xz, q.y - 0.8 + 0.2 * q.x * q.x).xzy, vec3 (2., 0.4, 0.1), rc));
  q.z = abs (q.z);
  q -= vec3 (-2.5, -1., 1.1);
  d = min (d, PrRoundCylDf (q.xzy, 0.7 * (1. - 0.05 * q.y), rc, 1.5));
  dMin = min (dMin, d);
  nStr = 4.;
  return dMin;
}

float PyrBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d;
  q = p;
  q.xz = Rot2D (q.xz, 2. * pi * (floor (4. * (pA - 0.5) + 0.5) / 4.));
  q.xy -= vec2 (-3.5, 2.);
  d = max (max (abs (q.y) - 2., - q.x + q.y), min (0.4 - abs (abs (q.z) - 0.6), (q.y - q.x + 0.18 -
     abs (mod (q.x + q.y - 0.02, 0.4) - 0.2) / sqrt2)));
  dMin = min (dMin, d);
  q = p;
  q.y -= 4.2;
  d = PrCylAnDf (q.xzy, 0.5, 0.05, 0.2);
  dMin = min (dMin, d);
  nStr = 4.;
  return dMin;
}

float TajBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d;
  q = p;
  d = PrRoundBoxDf (q, vec3 (5.8, 0.1, 5.8) - rc, rc);
  dMin = min (dMin, d);
  q.xz = Rot2D (q.xz, 2. * pi * (floor (8. * (pA - 0.5) + 0.5) / 8.));
  q.xy -= vec2 (-3.5, 2.);
  d = abs (q.y) - 2.;
  q.y = mod (q.y + 0.4, 2.) - 1.;
  q.z = abs (q.z) - 0.6;
  d = max (d, max (- q.x, - max (PrCapsDf ((q - vec3 (0., -0.5, 0.)).xzy, 0.4, 1.), - q.y - 0.2)));
  dMin = min (dMin, d);
  q = p;
  q.xz = Rot2D (q.xz, 2. * pi * (floor (4. * (pA - 0.5) + 0.5) / 4.));
  q.z = abs (q.z);
  q -= vec3 (-5., 3.1, 5.);
  d = PrCylDf (q.xzy, 0.3 - 0.1 * q.y / 3., 3.);
  dMin = min (dMin, d);
  q.y -= 3.2;
  d = PrSphDf (q, 0.3);
  dMin = min (dMin, d);
  q = p;
  q.y -= 4.6;
  d = PrSphDf (q, 2.);
  dMin = min (dMin, d);
  nStr = 1.;
  return dMin;
}

float PisBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d, ht, dh, a, ai, nf, na;
  nf = 8.;
  na = 12.;
  ht = 3.5;
  q = p;
  d = PrCylDf (q.xzy, 3., 0.05);
  dMin = min (dMin, d);
  q.xz = Rot2D (q.xz, pi / 4.);
  q.y -= ht + 0.05;
  q.xy = Rot2D (q.xy, 0.025 * pi);
  a = atan (q.z, - q.x) / (2. * pi) + 0.5;
  d = PrCylDf (q.xzy, 1., ht);
  dh = abs (q.y) - ht * (1. + 1. / (4. * nf));
  q.y = mod (q.y + ht / nf, 2. * ht / nf) - ht / nf;
  d = min (d, max (dh, PrRoundCylDf (q.xzy, 1.2, rc, ht / (4. * nf))));
  ai = floor (na * (a - 0.5) + 0.5) / na;
  q.xz = Rot2D (q.xz, 2. * pi * ai);
  q.y = abs (q.y) - ht / nf;
  d = max (d, - PrRoundBoxDf (vec3 (length (q.xz) - 1., q.y, 2. * pi * (abs (a - ai - 0.5) - 0.5 / na)),
     vec3 (0.2, 0.55 * ht / nf, 0.12) - rc, rc));
  q.x -= -1.05;
  d = min (d, max (dh, PrCylDf (q.xzy, 0.08, ht / nf)));
  dMin = min (dMin, d);
  q = p;
  q.y -= 0.4;
  d = PrRoundBox2Df (vec2 (pR - 6.5, abs (q.y) - 0.4), vec2 (0.25, 0.01), rc);
  q.xz = Rot2D (q.xz, 2. * pi * (floor (64. * (pA - 0.5) + 0.5) / 64.));
  q.x = abs (q.x + 6.5) - 0.15;
  d = min (d, PrCylDf (q.xzy, 0.08, 0.4));
  dMin = min (dMin, d);
  nStr = 3.;
  return dMin;
}

float FrtBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d, r;
  q = p;
  q.xz = Rot2D (q.xz, 2. * pi * (floor (5. * (pA - 0.5) + 0.5) / 5.));
  q.xy -= vec2 (-2., 1.4);
  d = max (max (- q.x, abs (q.y) - 1.4), - PrRoundBoxDf (vec3 (q.x, abs (q.y) - 0.5,
     mod (q.z + 0.5, 1.) - 0.5), vec3 (0.1, 0.2, 0.15), rc));
  d = min (d, PrRoundBox2Df (vec2 (q.x - 2., q.y - 1.4 - 0.3 * q.x), vec2 (2.2, 0.06) - rc, rc));
  dMin = min (dMin, d);
  q.xy -= vec2 (-3.5, -0.6);
  d = max (max (abs (q.x) - 0.4, abs (q.y) - 0.8),
     - PrRoundBox2Df (vec2 (q.yz - vec2 (-0.2, 0.)), vec2 (0.4, 0.3), rc));
  d = min (d, PrRoundBox2Df (vec2 (q.x, q.y - 0.9 + 0.4 * abs (q.x)), vec2 (0.6, 0.06) - rc, rc));
  dMin = min (dMin, d);
  q.y -= 0.3;
  q.z = abs (q.z) - 5.5 * tan (pi / 5.);
  d = PrCylDf (q.xzy, 0.8, 1.1);
  r = 0.2 * length (q.xz);
  q.y -= 1.4 - r;
  d = min (d, PrCylDf (q.xzy, 1.1, 0.3 - r));
  dMin = min (dMin, d);
  nStr = 5.;
  return dMin;
}

float WvnBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d, rs, s, h, f, yy;
  h = 2.5;
  rs = 0.15;
  q = p;
  q.y -= h + rs;
  f = q.y / h - 1.;
  yy = abs (q.y) - h;
  d = max (pR - 2.3 - 0.7 * f * f, yy);
  q.xz *= 1.4 + 0.4 * f;
  q.xz = vec2 (20. * pA, length (q.xz) - 10. / pi);
  d = min (d, length (vec2 (yy, q.z)) - 2. * rs);
  s = 0.4 * rs * cos (2. * pi * q.x);
  d = min (d, max (min (length (vec2 ((mod ((q.x + q.y) + 0.5, 1.) - 0.5) / sqrt2, q.z + s)),
     length (vec2 ((mod ((q.x - q.y) + 0.5, 1.) - 0.5) / sqrt2, q.z - s))) - rs, yy));
  dMin = min (dMin, d);
  nStr = 3.;
  return 0.5 * dMin;
}

float PstBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d, s, ss;
  s = 1.7;
  q = p;
  q.xz = Rot2D (q.xz, 0.25 * pi);
  for (int j = VAR_ZERO; j < 5; j ++) {
    q.xz = Rot2D (q.xz, 0.25 * pi);
    q.xz = abs (q.xz) - 0.75 * s;
    ss = s * s;
    q.xz = mix (q.xz, q.zx, step (q.x, q.z));
    d = min (PrRoundBoxDf (q - vec3 (0., 0.6 * ss - rc, 0.), s * vec3 (0.3, 0.6 * s, 0.3) - rc, rc),
       PrCylDf ((q - vec3 (0., 1.45 * ss - rc, 0.)).xzy, 0.12 * s, 0.25 * ss));
    dMin = min (dMin, d);
    s *= 0.85;
  }
  nStr = 4.;
  return dMin;
}

float BrgBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d;
  q = p;
  q.y -= 0.1 + 0.01 * sin (8. * pi * pR);
  d = max (abs (abs (pR - 5.2) - 1.4) - 0.4, abs (q.y) - 0.1);
  dMin = min (dMin, d);
  q = p;
  q.y -= 1.5;
  d = min (PrCylDf (q.xzy, 2.5, 1.5),
     PrCylDf (vec3 (q.xz, q.y - (2.2 - 0.25 * pR + 0.1 * sin (32. * pi * pA))), 2.8, 0.07));
  q.y -= 2.16;
  d = min (d, PrCylDf (q.xzy, 0.5, 0.22));
  q = p;
  q.xz = Rot2D (q.xz, 2. * pi * (floor (3. * (pA - 0.5) + 0.5) / 3.));
  d = max (d, - PrRoundBoxDf (q - vec3 (-2.5, 0.8, 0.), vec3 (0.7, 0.8, 0.7), rc));
  dMin = min (dMin, d);
  q.x += 5.2;
  q.y -= 0.5 - 0.2 * q.x * q.x;
  d = PrBoxDf (q, vec3 (1.6, 0.1, 0.6));
  q.z = abs (q.z);
  q.yz -= vec2 (0.3, 0.5);
  d = min (d, PrRoundBoxDf (q, vec3 (1.6, 0.3, 0.1) - rc, rc));
  dMin = min (dMin, d);
  nStr = 3.;
  return dMin;
}

float PolBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d, pRad, bRad, cRad;
  pRad = 4.;
  bRad = 0.4;
  cRad = 0.25;
  p.y -= 0.65 * pRad;
  q = DodSym (p);
  q.z += pRad;
  d = PrSphDf (q, bRad);
  q = IcoSym (p);
  q.z += pRad;
  d = min (d, PrSphDf (q, bRad));
  q.yx = sin (mod (atan (q.x, q.y) + pi / 3., 2. * pi / 3.) + vec2 (0.5 * pi, 0.)) * length (q.xy);
  q.z = dot (q.yz, sin (- pi / 10. + vec2 (0., 0.5 * pi)));
  d = SmoothMin (SmoothMin (d, length (q.xz) - cRad, rc), PrSphDf (p, pRad - 0.1), rc);
  dMin = min (dMin, d);
  nStr = 5.;
  return dMin;
}

float PanBldgDf (vec3 p, float dMin)
{
  vec3 q;
  float d;
  p.y -= 1.8;
  q = p;
  d = PrRoundBoxDf (q, vec3 (4.9, 1.6, 2.9) - rc, rc);
  q.y = abs (q.y) - 1.5;
  d = min (d, PrRoundBoxDf (q, vec3 (5.4, 0.1, 3.4) - rc, rc));
  q = p;
  q.y -= -1.7;
  d = min (d, PrRoundBoxDf (q, vec3 (6., 0.1, 4.) - rc, rc));
  dMin = min (dMin, d);
  q = p;
  q.y -= 2.2;
  d = PrCylDf (q.xzy, 1.7, 0.75);
  q.y -= 0.75;
  d = min (d, max (max (PrSphDf (q, 2.), - q.y), 0.2 - pR));
  dMin = min (dMin, d);
  q = p;
  q.xz = mod (q.xz + 0.5, 1.) - 0.5;
  d = max (PrCylDf (q.xzy, 0.2, 1.5), max (abs (p.x) - 5.5, abs (p.z) - 3.5));
  q = p;
  q.xz = Rot2D (q.xz, 2. * pi * (floor (16. * (pA - 0.5) + 0.5) / 16.));
  q.xy -= vec2 (-1.85, 2.2);
  d = min (d, PrCylDf (q.xzy, 0.15, 0.75));
  dMin = min (dMin, d);
  nStr = 2.;
  return dMin;
}

const int nPage = 12;

#define ISBOOK  1
#define ID_PAGE 1

float ObjDf (vec3 p)
{
  vec3 q;
  float dMin, d;
  dMin = dstFar;
  pR = length (p.xz);
  pA = ((pR > 0.) ? atan (p.z, - p.x) / (2. * pi) : 0.) + 0.5;
  nStr = 0.;
  q = p;
  q.y -= 2. * pileHt;
  rc = 0.05;
  if (idPage <= 4.) {
    if (idPage == 1.)       dMin = CasBldgDf (q, dMin);
    else if (idPage == 2.)  dMin = SprBldgDf (q, dMin);
    else if (idPage == 3.)  dMin = TwrBldgDf (q, dMin);
    else /*if (idPage == 4.)*/ dMin = PyrBldgDf (q, dMin);
  } else if (idPage <= 8.) {
    if (idPage == 5.)       dMin = TajBldgDf (q, dMin);
    else if (idPage == 6.)  dMin = PisBldgDf (q, dMin);
    else if (idPage == 7.)  dMin = FrtBldgDf (q, dMin);
    else /*if (idPage == 8.)*/ dMin = WvnBldgDf (q, dMin);
  } else /*if (idPage <= 12.)*/ {
    if (idPage == 9.)       dMin = PstBldgDf (q, dMin);
    else if (idPage == 10.) dMin = BrgBldgDf (q, dMin);
    else if (idPage == 11.) dMin = PolBldgDf (q, dMin);
    else /*if (idPage == 12.)*/ dMin = PanBldgDf (q, dMin);
  }
  idObj = 1;
  dMin = PileDf (p, dMin);
  return dMin;
}

float ObjRay (vec3 ro, vec3 rd)
{
  float dHit, d;
  dHit = 0.;
  for (int j = VAR_ZERO; j < 160; j ++) {
    d = ObjDf (ro + dHit * rd);
    if (d < 0.0001 || dHit > dstFar) break;
    dHit += d;
  }
  return dHit;
}

vec3 ObjNf (vec3 p)
{
  vec4 v;
  vec2 e;
  e = vec2 (0.0005, -0.0005);
  for (int j = VAR_ZERO; j < 4; j ++) {
    v[j] = ObjDf (p + ((j < 2) ? ((j == 0) ? e.xxx : e.xyy) : ((j == 2) ? e.yxy : e.yyx)));
  }
  v.x = - v.x;
  return normalize (2. * v.yzw - dot (v, vec4 (1.)));
}

float ObjSShadow (vec3 ro, vec3 rd)
{
  float sh, d, h;
  sh = 1.;
  d = 0.02;
  for (int j = VAR_ZERO; j < 30; j ++) {
    h = ObjDf (ro + d * rd);
    sh = min (sh, smoothstep (0., 0.02 * d, h));
    d += clamp (3. * h, 0.02, 0.3);
    if (sh < 0.05) break;
  }
  return 0.5 + 0.5 * sh;
}

float ObjAO (vec3 ro, vec3 rd)
{
  float ao, d;
  ao = 0.;
  for (int j = VAR_ZERO; j < 4; j ++) {
    d = float (j + 1) / 8.;
    ao += max (0., d - 3. * ObjDf (ro + d * rd));
  }
  return 0.5 + 0.5 * clamp (1. - 0.2 * ao, 0., 1.);
}

float RippleHt (vec2 p)
{
  vec2 q;
  float s1, s2;
  q = Rot2D (p, -0.02 * pi);
  s1 = abs (sin (4. * pi * abs (q.y + 1.5 * Fbm2 (0.7 * q))));
  s1 = (1. - s1) * (s1 + sqrt (1. - s1 * s1));
  q = Rot2D (p, 0.01 * pi);
  s2 = abs (sin (3.1 * pi * abs (q.y + 1.9 * Fbm2 (0.5 * q))));
  s2 = (1. - s2) * (s2 + sqrt (1. - s2 * s2));
  return mix (s1, s2, 0.1 + 0.8 * smoothstep (0.3, 0.7, Fbm2 (2. * p)));
}

vec4 RippleNorm (vec2 p, vec3 vn, float f)
{
  vec2 e = vec2 (0.002, 0.);
  float h;
  h = RippleHt (p);
  vn.xy = Rot2D (vn.xy, f * (RippleHt (p + e) - h));
  vn.zy = Rot2D (vn.zy, f * (RippleHt (p + e.yx) - h));
  return vec4 (vn, h);
}

vec3 BgCol (vec3 rd)
{
  vec3 col;
  float f;
  if (rd.y == 0.) rd.y = 0.0001;
  f = Fbm2 (4. * rd.xz / rd.y);
  return mix (mix (vec3 (0.2, 0.3, 0.7), vec3 (0.5, 0.5, 0.7), pow (1. - max (rd.y, 0.), 8.)),
     vec3 (0.9), clamp (1.6 * f * rd.y + 0.1, 0., 1.));
}

vec3 SandCol (vec3 ro, vec3 rd, vec3 vn, float sh)
{
  vec4 vn4;
  vec3 col;
  float f;
  col = 0.8 * vec3 (1., 0.9, 0.7);
  if (ro.y < 0.05) {
    f = smoothstep (0.85, 0.9, vn.y);
    vn4 = RippleNorm (ro.xz, vn, 8. * f * (1. - smoothstep (-0.4, -0.2, dot (rd, vn))));
    vn = vn4.xyz;
    col *= mix (1., 0.9 + 0.1 * smoothstep (0.1, 0.3, vn4.w), f);
  }
  vn = VaryNf (16. * ro, vn, 1.);
  col *= (0.8 + 0.2 * Fbm3 (32. * ro)) *
     (0.2 + 0.1 * max (vn.y, 0.) + 0.8 * sh * max (dot (vn, sunDir), 0.));
  return col;
}

vec3 ShowScene (vec3 ro, vec3 rd)
{
  vec4 vn4;
  vec3 col, vn;
  float dstObj, sh, ao, f, fr, a, s, w;
  int idObjT;
  bool isSand;
  isSand = false;
  pgRnd = 17.11 * idPage;
  dstObj = ObjRay (ro, rd);
  if (dstObj < dstFar) {
    ro += dstObj * rd;
    vn = ObjNf (ro);
    idObjT = idObj;
    sh = ObjSShadow (ro, sunDir);
    if (idObjT == 1) {
      ao = ObjAO (ro, vn);
      vn = VaryNf (32. * ro, vn, 0.5);
      sh = min (sh, ao);
      col = vec3 (1., 0.9, 0.7);
      col *= 0.8 + 0.2 * Fbm3 (32. * ro);
      col = (col * (ao * 0.2 + 0.8 * sh * max (dot (vn, sunDir), 0.)) +
         0.05 * step (0.95, sh) * pow (max (0., dot (sunDir, reflect (rd, vn))), 32.));
    } else if (idObjT == 2) {
      isSand = true;
    }
  } else if (rd.y < 0.) {
    f = 0.1 * tCur;
    ro += (- ro.y / rd.y) * rd;
    a = atan (ro.z, - ro.x) / (2. * pi) + 0.5;
    fr = mix (Fbm1 (pgRnd + 32. * a), Fbm1 (pgRnd + 32. * (a - 1.)), a);
    s = SmoothBump (0.25, 0.75, 0.25, mod (f + 0.4, 1.));
    w = length (ro.xz) - (grRad + 0.2 + mix (16. * fr, 16. + 3. * fr, s));
    if (w < 0.) {
      vn = vec3 (0., 1., 0.);
      sh = 1.;
      isSand = true;
    } else {
      w = 0.02 * w + f;
      col = mix (mix (vec3 (0.2, 0.3, 1.), vec3 (0.2, 0.3, 1.) + vec3 (0.6, 0.5, 0.),
         Fbm2 (2048. * vec2 (w, fr)) * SmoothBump (0.4, 0.6, 0.01, mod (w, 1.))), vec3 (0.2, 0.3, 1.),
         smoothstep (-0.1, 0., rd.y));
    }
  } else {
    col = BgCol (rd);
  }
  if (isSand) col = SandCol (ro, rd, vn, sh);
  return clamp (col, 0., 1.);
}

void ExMain  (out vec3 fCol, in vec2 fCoord)
{
  mat3 vuMat;
  vec3 ro, rd;
  float el, az, zmFac;
  az = 0.2 * pi;
  el = -0.05 * pi;
  az += (msAz > -999.) ? msAz : 0.;
  el += (msEl > -999.) ? msEl : 0.;
  el = clamp (el, -0.4 * pi, -0.01 * pi);
  vuMat = StdVuMat (el, az);
  grRad = 16.;
  pileHt = 1.5;
  ro = vuMat * vec3 (0., 3., -40.);
  ro.y = max (ro.y, 0.2);
  zmFac = 4.;
  dstFar = 100.;
  rd = vuMat * normalize (vec3 (fCoord, zmFac));
  sunDir = vuMat * normalize (vec3 (1., 1., -1.));
  fCol = pow (ShowScene (ro, rd), vec3 (0.8));
}

#if ISBOOK

float ExObjDf (vec3 p)
{
  vec3 q;
  float dMin, d, rRad, hRad, hOff, np, bc, thFac, nRing, y, s, w;
  dMin = dstFarEx;
  rRad = 0.4;
  hRad = 0.08;
  nRing = 4.;
  hOff = 0.15;
  thFac = 2.2;
  np = float (nPage - 1);
  q = p;
  s = mod (nRing, 2.);
  w = pgSize.z / nRing;
  bc = q.z;
  bc = mod (bc + s * w, 2. * w) - w;
  for (int k = VAR_ZERO; k < nPage; k ++) {
    q = p;
    q.y -= 0.5 * np * thFac * pgSize.y;
    y = q.y + np * thFac * pgSize.y * phsTurn;
    if (k == 0) q.xy = Rot2D (vec2 (q.x, y), -2. * pi * phsTurn);
    else q.y -= thFac * pgSize.y * (phsTurn - float (k));
    d = hRad - length (vec2 (q.x - rRad, bc));
    q.x -= pgSize.x - hOff + rRad;
    d = max (PrRoundBoxDf (q, pgSize - pgSize.y, pgSize.y), d);
    DMINQ (k + 1);
  }
  q = p;
  d = abs (q.z) - pgSize.z;
  q.z = bc;
  d = max (PrTorusDf (q, 0.5 * hRad, rRad + 0.3 * hRad), d);
  DMINQ (nPage + 1);
  return dMin;
}

float ExObjRay (vec3 ro, vec3 rd)
{
  float dHit, d;
  dHit = 0.;
  for (int j = 0; j < 120; j ++) {
    d = ExObjDf (ro + dHit * rd);
    if (d < 0.0005 || dHit > dstFarEx) break;
    dHit += d;
  }
  return dHit;
}

vec3 ExObjNf (vec3 p)
{
  vec4 v;
  vec2 e;
  e = vec2 (0.001, -0.001);
  for (int j = VAR_ZERO; j < 4; j ++) {
    v[j] = ExObjDf (p + ((j < 2) ? ((j == 0) ? e.xxx : e.xyy) : ((j == 2) ? e.yxy : e.yyx)));
  }
  v.x = - v.x;
  return normalize (2. * v.yzw - dot (v, vec4 (1.)));
}

float ExObjSShadow (vec3 ro, vec3 rd)
{
  float sh, d, h;
  sh = 1.;
  d = 0.02;
  for (int j = VAR_ZERO; j < 30; j ++) {
    h = ExObjDf (ro + d * rd);
    sh = min (sh, smoothstep (0., 0.05 * d, h));
    d += h;
    if (sh < 0.05) break;
  }
  return 0.7 + 0.3 * sh;
}

vec3 ExShowScene (vec3 ro, vec3 rd)
{
  vec4 col4;
  vec3 col, c, vn;
  vec2 s, w;
  float dstObj, sh, npCyc, tpCyc, tpWait, nDotL, t;
  bool isImg;
  pgSize = vec3 (3.2, 0.011, 1.8);
  tpCyc = 3.;
  tpWait = 4.;
  t = (tpBook < 0.) ? (- tpBook * float (nPage - 1) * (tpCyc + tpWait)) :
     mod (tpBook - tpWait, float (nPage) * (tpCyc + tpWait));
  npCyc = mod (floor (t / (tpCyc + tpWait)), float (nPage));
  phsTurn = min (mod (t / (tpCyc + tpWait), 1.) * (tpCyc + tpWait) / tpCyc, 1.);
  isImg = false;
  dstObj = ExObjRay (ro, rd);
  if (dstObj < dstFarEx) {
    ro += dstObj * rd;
    vn = ExObjNf (ro);
    nDotL = max (dot (vn, ltDirEx), 0.);
    if (idObj <= nPage) {
      idPage = mod ((float (idObj - 1) + npCyc), float (nPage)) + 1.;
      c = vec3 (0.9, 0.9, 0.8) * (0.85 + 0.15 * Fbm2 (8. * qHit.xz));
      col4 = vec4 (c, 0.2);
      s = pgSize.xz - abs (qHit.xz);
      if (qHit.y > 0.) {
        w = qHit.xz / pgSize.z;
        if (min (s.x, s.y) > 0.3) {
          isImg = true;
          ExMain (col, w);
        } else if (min (s.x, s.y) > 0.27) {
          col4 = vec4 (0.6 * c, 0.2);
        } else {
          col4 = vec4 (c, 0.2);
        }
      }
    } else if (idObj == nPage + 1) {
      col4 = vec4 (0.9, 0.9, 0.8, 0.2);
      nDotL *= nDotL;
    }
    sh = ExObjSShadow (ro + 0.001 * vn, ltDirEx);
    if (isImg) col *= 0.2 + 0.8 * sh;
    else col = col4.rgb * (0.2 + 0.8 * sh * nDotL) +
       col4.a * step (0.95, sh) * pow (max (dot (normalize (ltDirEx - rd), vn), 0.), 32.);
  } else {
    col = vec3 (1., 1., 0.9) * (0.5 + 0.2 * rd.y);
  }
  return clamp (col, 0., 1.);
}

#endif

void mainImage (out vec4 fragColor, in vec2 fragCoord)
{
  mat3 vuMat;
  vec4 mPtr;
  vec3 ro, rd, col;
  vec2 canvas, uv;
  float el, az, zmFac;
  canvas = iResolution.xy;
  uv = 2. * fragCoord.xy / canvas - 1.;
  uv.x *= canvas.x / canvas.y;
  tCur = iTime;
  mPtr = iMouse;
  mPtr.xy = mPtr.xy / canvas - 0.5;
#if ISBOOK
  tpBook = max (0.2 * tCur - 1., 0.);
  tpBook *= 3.;  //
  az = 0.;
  el = -0.5 * pi;
#else
  idPage = float (ID_PAGE);
#endif
  msAz = -999.;
  msEl = -999.;
  if (mPtr.z > 0.) {
    if (abs (mPtr.y) > 0.42) {
#if ISBOOK
      tpBook = - clamp (1.05 * mPtr.x + 0.45, 0., 1.);
#else
      idPage = clamp (floor ((1.05 * mPtr.x + 0.45) * float (nPage)), 0., float (nPage)) + 1.;
#endif
#if ISBOOK
    } else if (abs (mPtr.x - 0.04) > 0.42) {
      el += pi * mPtr.y * smoothstep (0.1, 0.15, abs (mPtr.y));
#endif
    } else if (abs (mPtr.x - 0.04) < 0.42 && abs (mPtr.y) < 0.42) {
      msAz = pi * clamp (3. * (mPtr.x - 0.04), -1., 1.);
      msEl = pi * mPtr.y;
    }
  }
#if ISBOOK
  vuMat = StdVuMat (el, az);
  ro = vec3 (3.2, 0., -20.);
  zmFac = 9.5;
  dstFarEx = 40.;
  ltDirEx = normalize (vec3 (1.));
  ro = vuMat * ro;
  rd = vuMat * normalize (vec3 (uv, zmFac));
  col = ExShowScene (ro, rd);
#else
  if (abs (uv.y) < 0.85) ExMain (col, uv);
  else col = vec3 (0.05);
#endif
  fragColor = vec4 (col, 1.);
}

float PrBoxDf (vec3 p, vec3 b)
{
  vec3 d;
  d = abs (p) - b;
  return min (max (d.x, max (d.y, d.z)), 0.) + length (max (d, 0.));
}

float PrRoundBoxDf (vec3 p, vec3 b, float r)
{
  return length (max (abs (p) - b, 0.)) - r;
}

float PrRoundBox2Df (vec2 p, vec2 b, float r)
{
  return length (max (abs (p) - b, 0.)) - r;
}

float PrSphDf (vec3 p, float r)
{
  return length (p) - r;
}

float PrCylDf (vec3 p, float r, float h)
{
  return max (length (p.xy) - r, abs (p.z) - h);
}

float PrCylAnDf (vec3 p, float r, float w, float h)
{
  return max (abs (length (p.xy) - r) - w, abs (p.z) - h);
}

float PrRoundCylDf (vec3 p, float r, float rt, float h)
{
  float dxy, dz;
  dxy = length (p.xy) - r;
  dz = abs (p.z) - h;
  return min (min (max (dxy + rt, dz), max (dxy, dz + rt)), length (vec2 (dxy, dz) + rt) - rt);
}

float PrCapsDf (vec3 p, float r, float h)
{
  return length (p - vec3 (0., 0., clamp (p.z, - h, h))) - r;
}

float PrTorusDf (vec3 p, float ri, float rc)
{
  return length (vec2 (length (p.xy) - rc, p.z)) - ri;
}

vec3 ComSym (vec3 p, vec2 cs1, vec2 cs2, float w)
{
  p.x = - abs (p.x);
  for (int j = 0; j < 4; j ++) {
    if (dot (p.yz, cs1) > 0.) p.zy = Rot2Cs (p.zy, cs2) * vec2 (1., -1.);
    if (j < 3) p.xy = Rot2D (p.xy, - w);
  }
  p.yx = sin (mod (atan (p.x, p.y) + 0.5 * w, w) - 0.5 * w + vec2 (0.5 * pi, 0.)) * length (p.xy);
  p.xz = - vec2 (abs (p.x), p.z);
  return p;
}

vec3 IcoSym (vec3 p)
{
  vec2 cs1, cs2;
  cs1 = cos (0.5 * acos (sqrt (5.) / 3.) + vec2 (0., 0.5 * pi));
  cs2 = cos (acos (sqrt (5.) / 3.) + vec2 (0., 0.5 * pi));
  p.yz = Rot2Cs (vec2 (p.y, abs (p.z)), cs1);
  p = ComSym (p, cs1, cs2, 2. * pi / 3.);
  return p;
}

vec3 DodSym (vec3 p)
{
  vec2 cs1, cs2;
  cs1 = cos (0.5 * atan (2.) + vec2 (0., 0.5 * pi));
  cs2 = cos (atan (2.) + vec2 (0., 0.5 * pi));
  p.xz = Rot2Cs (vec2 (p.x, abs (p.z)), vec2 (cs1.x, - cs1.y));
  p.xy = Rot2D (p.xy, - pi / 10.);
  p = ComSym (p, cs1, cs2, 2. * pi / 5.);
  return p;
}

mat3 StdVuMat (float el, float az)
{
  vec2 ori, ca, sa;
  ori = vec2 (el, az);
  ca = cos (ori);
  sa = sin (ori);
  return mat3 (ca.y, 0., - sa.y, 0., 1., 0., sa.y, 0., ca.y) *
         mat3 (1., 0., 0., 0., ca.x, - sa.x, 0., sa.x, ca.x);
}

vec2 Rot2D (vec2 q, float a)
{
  vec2 cs;
  cs = sin (a + vec2 (0.5 * pi, 0.));
  return vec2 (dot (q, vec2 (cs.x, - cs.y)), dot (q.yx, cs));
}

vec2 Rot2Cs (vec2 q, vec2 cs)
{
  return vec2 (dot (q, vec2 (cs.x, - cs.y)), dot (q.yx, cs));
}

float SmoothMin (float a, float b, float r)
{
  float h;
  h = clamp (0.5 + 0.5 * (b - a) / r, 0., 1.);
  return mix (b, a, h) - r * h * (1. - h);
}

float SmoothMax (float a, float b, float r)
{
  return - SmoothMin (- a, - b, r);
}

float SmoothBump (float lo, float hi, float w, float x)
{
  return (1. - smoothstep (hi - w, hi + w, x)) * smoothstep (lo - w, lo + w, x);
}

const float cHashM = 43758.54;

vec2 Hashv2f (float p)
{
  return fract (sin (p + vec2 (0., 1.)) * cHashM);
}

vec2 Hashv2v2 (vec2 p)
{
  vec2 cHashVA2 = vec2 (37., 39.);
  return fract (sin (dot (p, cHashVA2) + vec2 (0., cHashVA2.x)) * cHashM);
}

vec4 Hashv4v3 (vec3 p)
{
  vec3 cHashVA3 = vec3 (37., 39., 41.);
  return fract (sin (dot (p, cHashVA3) + vec4 (0., cHashVA3.xyz)) * cHashM);
}

float Noiseff (float p)
{
  vec2 t;
  float ip, fp;
  ip = floor (p);
  fp = fract (p);
  fp = fp * fp * (3. - 2. * fp);
  t = Hashv2f (ip);
  return mix (t.x, t.y, fp);
}

float Noisefv2 (vec2 p)
{
  vec2 t, ip, fp;
  ip = floor (p);  
  fp = fract (p);
  fp = fp * fp * (3. - 2. * fp);
  t = mix (Hashv2v2 (ip), Hashv2v2 (ip + vec2 (0., 1.)), fp.y);
  return mix (t.x, t.y, fp.x);
}

float Noisefv3 (vec3 p)
{
  vec4 t;
  vec3 ip, fp;
  ip = floor (p);
  fp = fract (p);
  fp *= fp * (3. - 2. * fp);
  t = mix (Hashv4v3 (ip), Hashv4v3 (ip + vec3 (0., 0., 1.)), fp.z);
  return mix (mix (t.x, t.y, fp.x), mix (t.z, t.w, fp.x), fp.y);
}

float Fbm1 (float p)
{
  float f, a;
  f = 0.;
  a = 1.;
  for (int j = 0; j < 5; j ++) {
    f += a * Noiseff (p);
    a *= 0.5;
    p *= 2.;
  }
  return f * (1. / 1.9375);
}

float Fbm2 (vec2 p)
{
  float f, a;
  f = 0.;
  a = 1.;
  for (int i = 0; i < 5; i ++) {
    f += a * Noisefv2 (p);
    a *= 0.5;
    p *= 2.;
  }
  return f * (1. / 1.9375);
}

float Fbm3 (vec3 p)
{
  float f, a;
  f = 0.;
  a = 1.;
  for (int j = 0; j < 5; j ++) {
    f += a * Noisefv3 (p);
    a *= 0.5;
    p *= 2.;
  }
  return f * (1. / 1.9375);
}

float Fbmn (vec3 p, vec3 n)
{
  vec3 s;
  float a;
  s = vec3 (0.);
  a = 1.;
  for (int j = 0; j < 5; j ++) {
    s += a * vec3 (Noisefv2 (p.yz), Noisefv2 (p.zx), Noisefv2 (p.xy));
    a *= 0.5;
    p *= 2.;
  }
  return dot (s, abs (n));
}

vec3 VaryNf (vec3 p, vec3 n, float f)
{
  vec3 g;
  vec2 e;
  e = vec2 (0.1, 0.);
  g = vec3 (Fbmn (p + e.xyy, n), Fbmn (p + e.yxy, n), Fbmn (p + e.yyx, n)) - Fbmn (p, n);
  return normalize (n + f * (g - n * dot (n, g)));
}

// --------[ Original ShaderToy ends here ]---------- //

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy - origin);
}