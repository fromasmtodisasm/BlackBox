#version 330 core

/*
 * Original shader from: https://www.shadertoy.com/view/lddfz2
 */

#ifdef GL_ES
precision mediump float;
#endif

// glslsandbox uniforms
uniform float time;
uniform vec2 resolution;

// shadertoy emulation
#define iTime time
#define iResolution resolution
const vec4 iMouse = vec4(0.);

// Emulate a black texture
#define texture(s, uv) vec4(1.0)

// --------[ Original ShaderToy begins here ]---------- //

#define PI				3.1415926535
#define SQRT2			0.707

#define LIGHT_DIR		normalize(vec3(1., -2., 1.))
#define CAM_SPEED		.3
#define CAM_POS 		vec3(4.*cos(-iTime*CAM_SPEED), 4.0, 4.*sin(-iTime*CAM_SPEED))



#define CYL_POS  vec3(0.56, 0.0, 0.56)
#define CYL_DIR  normalize(vec3(0., 1., 0.))
#define CYL_RAD  0.35
#define CYL_HEI  2.

#define SHADOW_SHARPNESS	.05

#define AA	4.


// strait from http://www.iquilezles.org/www/articles/boxfunctions/boxfunctions.htm
// added a parameter for the far intersection normal computation
vec2 boxIntersection( vec3 ro, vec3 rd, vec3 boxSize, mat4 txx, out vec3 outNormal, out vec3 outNormal2 )
{
    // convert from ray to box space
    vec3 rdd = (txx*vec4(rd,0.0)).xyz;
    vec3 roo = (txx*vec4(ro,1.0)).xyz;

    vec3 m = 1.0/rd;
    vec3 n = m*roo;
    vec3 k = abs(m)*boxSize;

    vec3 t1 = -n - k;
    vec3 t2 = -n + k;

    float tN = max( max( t1.x, t1.y ), t1.z );
    float tF = min( min( t2.x, t2.y ), t2.z );

    if( tN > tF) return vec2(-1.0); // no intersection

    outNormal = -sign(rdd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);
    
    vec3 d = abs(roo+rdd*tF);
    outNormal2 = -sign(rdd)*vec3(d.x>d.y&&d.x>d.z?1.:0., d.y>d.x&&d.y>d.z?1.:0., d.z>d.y&&d.z>d.x?1.:0.);

    return vec2( tN, tF );
}


vec2 traceCylindre(vec3 ro, vec3 rd, out vec3 n0, out vec3 n1)
{
    vec3 A = rd - CYL_DIR*dot(rd,CYL_DIR);
    vec3 B = ro - CYL_POS - CYL_DIR*dot(ro-CYL_POS,CYL_DIR);
    
    float delta = dot(A,B)*dot(A,B) - dot(A,A)*(dot(B,B)-CYL_RAD*CYL_RAD);
    
    if(delta < 0.)
        return vec2(1., 0.);
        
    delta = sqrt(delta);
    
    float t0 = (-dot(A,B)-delta)/dot(A,A);
    float t1 = (-dot(A,B)+delta)/dot(A,A);
    
    vec2 t = vec2(t0, t1);
    
    vec3 p0 = ro+rd*t0;
    float h0 = dot(CYL_DIR,p0 - CYL_POS);
    n0 = normalize(p0 - (CYL_POS+CYL_DIR*h0));
    if(h0 > 0.)
    {
        return vec2(1., 0);
    }
    
    if(h0 < -CYL_HEI)
    {
        n0 = vec3(0., -1., 0.);
        float tt = (-CYL_HEI-ro.y)/rd.y;
        
        vec3 p = ro+rd*tt;
        if(length(p - (CYL_POS - CYL_DIR*CYL_HEI))<CYL_RAD)
        {
            return vec2(tt, 100.);
        }
        return vec2(1., 0);
    }
    
    vec3 p1 = ro+rd*t1;
    float h1 = dot(CYL_DIR,p1 - CYL_POS);
    n1 = normalize(p1 - (CYL_POS+CYL_DIR*h1));
    
    return t;
}


// from iq   https://www.shadertoy.com/view/XlcSz2
float checkersTexture( in vec2 p )
{
    vec2 q = floor(p);
    float ret = mod( q.x+q.y, 2.0 );            // xor pattern
    
    if(p.x>2. && p.x<3. && p.y>2. && p.y<3.)
    {
        vec2 uv = p-vec2(2.);
        uv = uv.yx/16. + vec2(1., 11.)/16.;
        if(texture(iChannel0, uv).w<.5)
        	ret = 1.-ret;
    }
    
    if(p.x>4. && p.x<5. && p.y>1. && p.y<2.)
    {
        vec2 uv = p-vec2(4., 1.);
        uv = uv.yx/16. + vec2(2., 11.)/16.;
        if(texture(iChannel0, uv).w<.5)
        	ret = 1.-ret;
    }
    
    return ret;
}


// distance between lines + param along each vector to reach closest point
vec3 LineLineDist(vec3 P0, vec3 d0, vec3 P1, vec3 d1)
{
    vec3 d0c = d0 / dot(d0, d0);
    vec3 X = d1 - d0 * dot(d1, d0c);
    vec3 Y = P1 - P0 - d0*dot(P1, d0c);

    float t1 = -dot(Y, X) / dot(X, X);
    
    t1 = clamp(t1, -CYL_HEI, 0.);

    vec3 Pd1 = P1 + t1 * d1;
    
    float t0 = dot((Pd1 - P0), d0) / dot(d0, d0);
    vec3 Pd0 = P0 + t0 * d0;
    
    return vec3(length(Pd1-Pd0), dot(Pd0-P0,d0)/dot(d0,d0), dot(Pd1-P1, d1)/dot(d1,d1));
}

// simulates a circular focal
float shadowFunction(float x)
{
    //return x;
    x *= 0.999;
    x += 0.0005;
    return .5+(asin(x*2.-1.)+(x*2.-1.)*sqrt(1.-(x*2.-1.)*(x*2.-1.)))/PI;
}

// ray casting
vec3 render(in vec3 ro, in vec3 rd)
{
    
    float t = 10000.;
    vec3 col = vec3(1.);
    
    
    //--------------------------------------
    // Board
    vec3 n_1_box, n_2_box;
    vec2 t_box = boxIntersection(ro, rd, vec3(1., .12, 1.), mat4(1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1.), n_1_box, n_2_box);
    
    if(t_box.x < t_box.y)
    {
        t = t_box.x;
        
        float f = max(0., dot(n_1_box, LIGHT_DIR));
        f = f*.6+.4;
        
        vec3 pos = ro+rd*t;
        
        float checker = checkersTexture((pos.xz-vec2(-1.0001))*.5*4.999);
        
        
        
        //--------------------------------------
        // Shadow

        vec3 d_sha = LineLineDist(ro+rd*t, LIGHT_DIR, CYL_POS, CYL_DIR);
        
        float f_sha = 1.;
        if(d_sha.y > 0.)
        {
            f_sha = clamp(d_sha.x-CYL_RAD, -d_sha.y*SHADOW_SHARPNESS, d_sha.y*SHADOW_SHARPNESS)/d_sha.y/SHADOW_SHARPNESS * .5 + .5;
            f_sha = shadowFunction(f_sha);
            //col *= .5+.5*f_sha;
        }

        vec3 col_0 = vec3(0.9);
        vec3 col_1 = vec3(0.6);
        vec3 col_2 = vec3(0.2);
        
		vec3 colShadow = mix(col_1, col_2, checker);
		vec3 colLight  = mix(col_0, col_1, checker);
		vec3 baseCol   = mix(colShadow, colLight, f_sha);
        
        
        col = baseCol * f;
    }
    
    
    //--------------------------------------
    // Moving tile
    
    float t_tile = (-.12-ro.y)/rd.y;
    vec3 pos = ro+rd*t_tile;
    //vec2 tile_pos = mix(vec2(0.), vec2(.8, -.4), .5+.5*sin(iTime));
    vec2 tile_pos = mix(vec2(0., -.4), vec2(.8, -.8), .5+.5*sin(iTime));
    vec2 diff = pos.xz - tile_pos;

    if(abs(diff.x)<.2 && abs(diff.y)<.2)
    {
        vec3 n = vec3(0., -1., 0.);
        float f = max(0., dot(n, LIGHT_DIR));
        f = f*.6+.4;
        col = vec3(0.6)* f;
    }
    
    
    
    //--------------------------------------
    // Cylindre
    
    
    vec3 n_1_cyl, n_2_cyl;
    vec2 t_cyl = traceCylindre(ro, rd, n_1_cyl, n_2_cyl);
    
    if(t_cyl.x < t_cyl.y && t_cyl.x < t)
    {
        t = t_cyl.x;
        
        float f = max(0., dot(n_1_cyl, LIGHT_DIR));
        f = f*.6+.4;
        
        vec3 baseCol = vec3(28., 83., 48.)*2./255.;
        
        col = baseCol * f;
    }
    
    return col;
}


mat3 setCamera( in vec3 ro, in vec3 ta, float cr )
{
	vec3 cw = normalize(ta-ro);
	vec3 cp = vec3(sin(cr), cos(cr),0.0);
	vec3 cu = normalize( cross(cw,cp) );
	vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 p = (-iResolution.xy + 2.0*fragCoord.xy)/ iResolution.y;
    
    vec3 ca = normalize(vec3(-2., 3., 1.));
    
    if(iMouse.z > 0.)
    {
        float phi = (iMouse.x-0.5)/iResolution.x * PI * 2.0;
        float psi = ((iMouse.y-0.5)/iResolution.y) * PI * .5;

        ca = 5.0*vec3(cos(phi)*cos(psi), sin(psi), sin(phi)*cos(psi));
    }
        
    vec3 ta = vec3(0., .0, .0);
    mat3 m = setCamera(ca, ta, 0.0);
	
    float zoom = 1.;
    //vec3 ro = (m[0]*p.x + m[2]*p.y) / zoom;
    vec3 rd = m[1];
    
    
    // scene rendering
    vec3 col = vec3(0.);
    
    for(float i=0.; i<AA; ++i)
    for(float j=0.; j<AA; ++j)
    {
        vec2 o = vec2(i,j) / AA - 0.5;
        p += o/iResolution.y;
    	vec3 ro = (m[0]*p.x + m[2]*p.y) / zoom;
    	col += render( ro, rd);
    }
    
    col /= AA*AA;
    

    fragColor = vec4(col, 1.0);
}

// --------[ Original ShaderToy ends here ]---------- //

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}