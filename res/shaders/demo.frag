#version 330 core
/*
 * Original shader from: https://www.shadertoy.com/view/wdXBzs
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

// Emulate a black texture
#define texture(s, uv) vec4(0.0)
#define textureLod(s, uv, lod) vec4(0.0)
#define texelFetch(s, uv, lod) vec4(0.0)

// --------[ Original ShaderToy begins here ]---------- //
// Friday Giro Fever - Result of an improvised live code session on Twitch
// LIVE SHADER CODING, SHADER SHOWDOWN STYLE, EVERY TUESDAYS 21:00 Uk time:
// https://www.twitch.tv/evvvvil_

// "Being on the dole doesn't have to be dull." - William Shakespeare

vec2 z=vec2(0.),v=vec2(0.),e=vec2(.05,-.05);float t=0.,tt=0.,spo=0.,g=0.,gg=0.;vec3 np=vec3(0.),bp=vec3(0.),pp=vec3(0.),cp=vec3(0.),po=vec3(0.),no=vec3(0.),al=vec3(0.),ld=vec3(0.);//global vars. About as boring as being stuck in lockdown with your wife and kids.
float cy(vec3 p,vec3 r){return max(abs(length(p.xz)-r.x)-r.y,abs(p.y)-r.z/2.);} //my own primitive function which I didn't steal from IQ. Not bad for an ageing bitter skateboarder. It makes a hollow tube
mat2 r2(float r){return mat2(cos(r),sin(r),-sin(r),cos(r));}//rotate function. Short and sweet, just like Napoleon before his megalomanic fit.
vec2 fb( vec3 p,float s,float m) // fb "fucking bit" function make a base geometry which we use to make gyro and flying objects
{
  for(int i=0;i<4;i++){//bullshit kifs rotation cloner. "Friday Giro Fever" is a reference to the UK umemployment benefits, the "giro". It usually lands on friday, meaning there is always a party for everyone, every friday, in England.
    p=abs(p);//People against unemployment benefit are heartless like people annoyed at my humourous comments are boring
    p.xy*=r2(.48);//clone along xy axis with rotation
    p.yz*=r2(s*sin(p.z*.5)*.3);//this tweaks the gyro into much different shape for flying objects
  }
  vec2 h,t=vec2(cy(p,vec3(2,.3,.4)),m);//Make thin hollow tube either yellow or blue 
  h=vec2(cy(p,vec3(2,.1,.6)),6); //Make white hollow tube
  t=t.x<h.x?t:h; //Make toobs touch boobs! (blend geometry together while retaining nipple colour, I mean material ID)
  h=vec2(cy(p,vec3(2.,.4,.2)),3); //Make black hollow tube
  t=t.x<h.x?t:h; //Make toobs touch boobs! (blend geometry together while retaining nipple colour, I mean material ID)
  return t;
}
vec2 road( vec3 p) //Road is yellow circular bit in middle. Nah it doesn't lead to Rome, it leads to an industrial estate in Birmingham. What? You got something against burnt cars and the smell of petrol?
{ 
  vec2 h,t=vec2(cy(p,vec3(6,.5,1)),8);//Use my fancy fucking hollow tube primitive function, I know some clever bastard probably thought about it before but this is me we're talking about: I once got into an argument with the neighbour's dog and lost.
  h=vec2(cy(p,vec3(6,.5,5)),3);//Make another hollow toob, this time black and...
  h.x=abs(h.x)-.1;//...whip that fucker into shape, push it against the edges, and other sexual innuendos...
  h.x=max(h.x,abs(p.y)-.6); //croperoo the fuckeroo and whatch it groove
  t=t.x<h.x?t:h; //Make toobs touch boobs! (blend geometry together while retaining nipple colour, I mean material ID)
  t.x=max(t.x,-(abs(p.z)-.3)); //remove middle bit, otherwise it's full circle and so this way you can see it rotate more. Simple and easy like daddy in mommy
  h=vec2(cy(p,vec3(6.9,0,.1)),6); //One more hollow tube this time around the whole shit, make it glow, so it's nice a central focal point to the composition (look at me talking like I actually know what I'm doing... fucking hipster)
  g+=0.1/(0.1+h.x*h.x*100.);//Glow trick by Balkhan, which I tend to rinse and use as a party trick.
  t=t.x<h.x?t:h; //Make toobs touch boobs! (blend geometry together while retaining nipple colour, I mean material ID)
  return t;
}
vec2 mp( vec3 p )
{ 
  bp=np=cp=pp=p; //GYROBULLSHIT starts here
  bp.xy*=r2(tt); //We make two positions bp and np, one rotate clockwise the other anti-clockwise
  np.xy*=r2(-tt); //None of those positions are either right or wrong in terms of movements direction, get out of your hemisphere-centric bubble, Steve.
  vec2 h,t=fb(bp*.6,0.,5.); //Make first gyro bit and spin the fucker
  t.x/=0.6; //Since we scale the pos we must tweak domain into shape to avoid artifact
  t.x=max(t.x,-(abs(p.z)-.3)); //remove middle bit of outter gyro
  h=fb(np,0.,5.); //Make another fucking gyro and make it spin
  t=t.x<h.x?t:h;  //Mege both spinning gyros, bit like "Mr Speaker" trying to order the parliament with both parties spinning out of control around their own lame centre of political gravity
  h=fb(bp*1.7,0.,5.);h.x/=1.7;t=t.x<h.x?t:h; //Yeah yeah one more gyro, blah blah blah, something about distance fucking fields, man I'm so borded when is football restarting, being a geek is cool, but hooliganism is more rewarding.
  h=vec2(length(p)-.4);//Dumb fucking sphere in middle, make it glow, call the Tate Modern, tell them about my struggle as a child in our anonymous post-post-modern society and give me the Turner Prize.
  g+=0.1/(0.1+h.x*h.x*40.);t=t.x<h.x?t:h;//Make said dumb sphere glow, call Tate Modern again, explain I wasn't myself when I sent "that" email and and it's all due to Donald Trump's rage at China. Collect Turner Prize.
  h=vec2(length(p.xz+sin(p.y*20.)*.03)-(2.-cos(p.y*.15)*2.3));//Middle pointy erections. What? Yeah broh they are errected, so they are erections... You need to get laid more ma broski.
  h.x*=0.7; t=t.x<h.x?t:h; //Add those middle pointy erections... Mind where you sit.
  h=road(p);t=t.x<h.x?t:h; //Whole road thingy is added here.  
  pp.xz*=r2(tt);//Objects flying around, there is actually a sick position trick in there
  pp.xy=abs(pp.xy);//Rotate first, then abs symetry on xy axis
  pp+=vec3(cos(-3.+sin(tt))*6.,-2.,sin(-3.+sin(tt))*6.); //push position along circle
  pp.xz*=r2(sin(tt));//Rotate the fucking whole thing back and forth
  h=fb(pp*3.5,3.,7.);//And that's how we get satisfying pandular action. Please note "satisfying pandular action" is also what wives want.
  h.x*=0.15;  t=t.x<h.x?t:h;//since we scaled position, tweak back domain
  h=vec2(length(pp)-.2);//Blue glow sphere inside flying objects
  gg+=0.2/(0.1+h.x*h.x*(50.-sin(pp.z*.5+tt*2.)*48.)); //Make it glow and collect some Italia'90 football cards for your meak effort
  t=t.x<h.x?t:h; //Merge geom together, retain material ID 
  h=fb(p*.165,3.,7.); //Big outter shell is made of flying object body
  h.x*=3.0; t=t.x<h.x?t:h; //Scaled position means tweak domain. Should have gone to Stockton to become a tweaker when i had the opportunity back in 2010.
  h=vec2(length(p.xy)-.2+abs(p.z*.02)); //Long white line, usually taken on fridays, through a straw, while talking about how great a drummer Zach Hill really is.
  g+=0.2/(0.1+h.x*h.x*(50.-sin(pp.z*.2+tt*2.)*45.)); //Glow the fat white line along z axis, I know, I know, it is inviting. Who doesn't like to buy a gram of fun and chat shit to their friends?
  t=t.x<h.x?t:h;//Merge it all, get into rehab and become a "hero" for saving your own skin.
  return t; //I'm done commenting this and can go back to pointing menacingly at the neighbour's dog
}
vec2 tr( vec3 ro, vec3 rd,float _max,int iter) // main trace / raycast / raymarching loop function 
{
  vec2 h,t= vec2(.1); //Near plane because when it all started the hipsters still lived in Norwich and they only wore tweed.
  for(int i=0;i<128;i++){ //Main loop de loop 
    if (i>=iter) break;
    h=mp(ro+rd*t.x); //Marching forward like any good fascist army: without any care for culture theft. (get distance to geom)
    if(h.x<.00001||t.x>_max) break;//Conditional break we hit something or gone too far. Don't let the bastards break you down!
    t.x+=h.x;t.y=h.y; //Huge step forward and remember material id. Let me hold the bottle of gin while you count the colours.
  }
  if(t.x>_max) t.y=0.;//If we've gone too far then we stop, you know, like Alexander The Great did when he realised his wife was sexting some Turkish bloke. (10 points whoever gets the reference)
  return t;
}
// Rough shadertoy approximation of the bonzomatic noise texture by yx - https://www.shadertoy.com/view/tdlXW4
vec4 texNoise(vec2 uv){ float f = 0.; f+=texture(iChannel0, uv*.125).r*.5;
    f+=texture(iChannel0,uv*.25).r*.25;f+=texture(iChannel0,uv*.5).r*.125;
    f+=texture(iChannel0,uv*1.).r*.125;f=pow(f,1.2);return vec4(f*.45+.05);
}
#define a(d) clamp(mp(po+no*d).x/d,0.,1.)
#define s(d) smoothstep(0.,1.,mp(po+ld*d).x/d)
void mainImage( out vec4 fragColor, in vec2 fragCoord )//2 lines above are a = ambient occlusion and s = sub surface scattering
{
  vec2 uv=(fragCoord.xy/iResolution.xy-0.5)/vec2(iResolution.y/iResolution.x,1); //get UVs, nothing fancy, 
  tt=mod(iTime,62.82);  //Time variable, modulo'ed to avoid ugly artifact. Imagine moduloing your timeline, you would become a cry baby straight after dying a bitter old man. Christ, that's some fucking life you've lived, Steve.
  v=mix(vec2(3.,8.8),vec2(12.,16.),ceil(cos(tt*.4)));//Reuse the v variable as holder of camera variables
  vec3 ro=mix(vec3(1),vec3(-1,-.8,-1),ceil(sin(tt*.4)))*vec3(cos(tt*.2)*v.y,v.x+sin(tt*.4)*2.,sin(tt*.2)*v.y),//Ro=ray origin=camera position We build camera right here broski. Gotta be able to see, to peep through the keyhole.
  cw=normalize(vec3(0)-ro),cu=normalize(cross(cw,vec3(0,1,0))),cv=normalize(cross(cu,cw)), //camera forward, left and up vector.
  rd=mat3(cu,cv,cw)*normalize(vec3(uv,.5)),co,fo;//rd=ray direction (where the camera is pointing), co=final color, fo=fog color
  ld=normalize(vec3(.2,.5,-.1)); //ld=light direction
  co=fo=vec3(.09)-length(uv*.8)*.107;//background is dark with vignette
  z=tr(ro,rd,50.,128);t=z.x; //Trace the trace in the loop de loop. Sow those fucking ray seeds and reap them fucking pixels.
  if(z.y>0.){ //Yeah we hit something, unlike you trying to throw a spear at a pig. We wouldnt have survive the ice age with you and your nerdy mates.
    po=ro+rd*t; //Get ray pos, know where you at, be where you is.
    no=normalize(e.xyy*mp(po+e.xyy).x+e.yyx*mp(po+e.yyx).x+e.yxy*mp(po+e.yxy).x+e.xxx*mp(po+e.xxx).x); //Make some fucking normals. You do the maths while I count how many instances of Holly Willoughby there really is.
    spo=20.+20.*exp2(10.*texNoise(vec2(bp.z,dot(bp.xy,vec2(.7)))*.3).r);//spo is specular power and refraction index
    al=vec3(1.,.5,0);//albedo is base colour by default it's yellow 
    if(z.y<5.) spo=40.,al=vec3(0); //material system if less than 5 make it black, with specular power 40 and no reflections
    if(z.y>5.) spo=40.,al=vec3(1); //material system if more than 5 make it white, with specular power 40 and no reflections
    if(z.y>6.) al=mix(vec3(.1,.2,.4),vec3(.1,.35,.6),sin(cp*.5)*.5+.5); //Material Id more than 6 makes it blue / green gradient
    if(z.y>7.) al=vec3(1.,.5,0),spo=20.+20.*exp2(10.*texNoise(vec2(cp.z,dot(cp.xy,vec2(.7)))*.3).r); //Material ID more than 7 makes it yellow and we re adapt the specular power and refraction index
    float dif=max(0.,dot(no,ld)), //Dumb as fuck diffuse lighting
    fr=pow(1.+dot(no,rd),4.), //Fr=fresnel which adds background reflections on edges to composite geometry better
    sp=pow(max(dot(reflect(-ld,no),-rd),0.),spo);//Sp=specular, stolen from shane
    co=mix(sp+mix(vec3(.8),vec3(1.),abs(rd))*al*(a(.1)*a(.2)+.2)*(dif+s(13.)),fo,min(fr,0.2));//Building the final lighting result, compressing the fuck outta everything above into an RGB shit sandwich
    if(z.y>7.||z.y==5.){ //If material id is 5 or more than 7 then we do reflections
      rd=refract(rd,-no,1.-(spo*.0005));//reflect rd with refraction
      z=tr(po+rd*0.01,rd,50.,80); //Throw another ray from surface, shift ray start position a bit and shoot
      if(z.y>0.){ //If we hit something in reflection ray... (optimization and cleanup this)
        po=po+rd*z.x; //Get where we at
        no=no=normalize(e.xyy*mp(po+e.xyy).x+e.yyx*mp(po+e.yyx).x+e.yxy*mp(po+e.yxy).x+e.xxx*mp(po+e.xxx).x);//More broing normals calculations
        al=vec3(1.,.5,0);//Gotta give it the material id thing so we know how to colour what
        if(z.y<5.) al=vec3(0);//ya ya give it back the colours
        if(z.y>5.) al=vec3(1);//Still pretty boring shit
        if(z.y>6.) al=vec3(.1,.2,.4);//Did you know Louie Theroux had interviewed Tiger King before he went to prison?
        if(z.y>7.) al=vec3(1.,.5,0); 
        float dif=max(0.,dot(no,ld));
        co=co+(dif*al*fr);
      }
    }
    co=mix(fo,co,exp(-.0001*t*t*t)); //Fog soften things, but it won't stop your mother from being unimpressed by your current girlfriend
  } 
  fragColor = vec4(pow(co+g*.2+gg*.3*vec3(.0,.1,.7),vec3(.45)),1);
}
// --------[ Original ShaderToy ends here ]---------- //

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy - origin);
}