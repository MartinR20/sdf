#version 130

#define PI 3.1415926538

uniform vec2 WindowSize;
uniform float Time;

in vec4 fragmentColor;
out vec4 fragColor;

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;

/**
 * Rotation matrix around the X axis.
 */
mat3 rotateX(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(1, 0, 0),
        vec3(0, c, -s),
        vec3(0, s, c)
    );
}

/**
 * Rotation matrix around the Y axis.
 */
mat3 rotateY(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, 0, s),
        vec3(0, 1, 0),
        vec3(-s, 0, c)
    );
}

/**
 * Rotation matrix around the Z axis.
 */
mat3 rotateZ(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, -s, 0),
        vec3(s, c, 0),
        vec3(0, 0, 1)
    );
}

float sphere(vec3 p, float r) {
    return length(p) - r;
}

float ellipse(vec3 p, vec3 r)
{
    float k1 = length(p/r);
    float k2 = length(p/(r*r));
    return k1*(k1-1.0)/k2;
}


float sminCubic( float a, float b, float k )
{
    float h = max( k-abs(a-b), 0.0 )/k;
    return min( a, b ) - h*h*h*k*(1.0/6.0);
}

float mix(float a, float b, float t) {
   return t*a + (1-t)*b;
}

vec2 sceneSDF(vec3 p) {
   vec3 _p = p;
   float d = p.y;
   float prevd = MAX_DIST;
   float m = 1.0;
   
   d = sphere(_p + vec3(0.f, -0.5f, 0.f), 1);       
   d = sminCubic(sphere(_p + vec3(0.f, 0.5f, 0.3f), 0.5f), d, 0.9);    

   if(d < prevd) m = 2.0;   
   prevd = d;

   d = min(sphere(_p + vec3(0.4f, -0.8f, 0.7f), 0.3f), d);
   d = min(sphere(_p + vec3(-0.4f, -0.8f, 0.7f), 0.3f), d);

   if(d < prevd) m = 3.0;   

   return vec2(d, m);
}

/**
 * Return the shortest distance from the eyepoint to the scene surface along
 * the marching direction. If no part of the surface is found between start and end,
 * return end.
 * 
 * eye: the eye point, acting as the origin of the ray
 * marchingDirection: the normalized direction to march in
 * start: the starting distance away from the eye
 * end: the max distance away from the ey to march before giving up
 */
vec2 shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, 
                               float start, float end) {
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        vec2 dist = sceneSDF(eye + depth * marchingDirection);
        if (dist.x < EPSILON) {
	   return vec2(depth, dist.y);
        }
        depth += dist.x;
        if (depth >= end) {
            return vec2(end, 1.0);
        }
    }
    return vec2(end, 1.0);
}
            

/**
 * Return the normalized direction to march in from the eye point for a single pixel.
 * 
 * fieldOfView: vertical field of view in degrees
 * size: resolution of the output image
 * fragCoord: the x,y coordinate of the pixel in the output image
 */
vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}

/**
 * Using the gradient of the SDF, estimate the normal on the surface at point p.
 */
vec3 estimateNormal(vec3 p) {
    return normalize(vec3(
        sceneSDF(vec3(p.x + EPSILON, p.y, p.z)).x - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)).x,
        sceneSDF(vec3(p.x, p.y + EPSILON, p.z)).x - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)).x,
        sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)).x - sceneSDF(vec3(p.x, p.y, p.z - EPSILON)).x
    ));
}

/**
 * Lighting contribution of a single point light source via Phong illumination.
 * 
 * The vec3 returned is the RGB color of the light's contribution.
 *
 * k_a: Ambient color
 * k_d: Diffuse color
 * k_s: Specular color
 * alpha: Shininess coefficient
 * p: position of point being lit
 * eye: the position of the camera
 * lightPos: the position of the light
 * lightIntensity: color/intensity of the light
 *
 * See https://en.wikipedia.org/wiki/Phong_reflection_model#Description
 */
vec3 phongContribForLight(vec3 k_d, vec3 k_s, float alpha, vec3 p, 
                          vec3 eye, vec3 lightPos, vec3 lightIntensity) {
    vec3 N = estimateNormal(p);
    vec3 L = normalize(lightPos - p);
    vec3 V = normalize(eye - p);
    vec3 R = normalize(reflect(-L, N));
    
    float dotLN = dot(L, N);
    float dotRV = dot(R, V);
    
    if (dotLN < 0.0) {
        // Light not visible from this point on the surface
        return vec3(0.0, 0.0, 0.0);
    } 
    
    if (dotRV < 0.0) {
        // Light reflection in opposite direction as viewer, apply only diffuse
        // component
        return lightIntensity * (k_d * dotLN);
    }
    return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

/**
 * Lighting via Phong illumination.
 * 
 * The vec3 returned is the RGB color of that point after lighting is applied.
 * k_a: Ambient color
 * k_d: Diffuse color
 * k_s: Specular color
 * alpha: Shininess coefficient
 * p: position of point being lit
 * eye: the position of the camera
 * rot: rotation for the light position
 *
 * See https://en.wikipedia.org/wiki/Phong_reflection_model#Description
 */
vec3 phongIllumination(vec3 k_a, vec3 k_d, vec3 k_s, float alpha, vec3 p, 
                       vec3 eye, mat3 rot) {
    const vec3 ambientLight = 0.5 * vec3(1.0, 1.0, 1.0);
    vec3 color = ambientLight * k_a;
    
    vec3 light1Pos = rot*vec3(4.0, 2.0, 4.0);
    vec3 light1Intensity = vec3(0.4, 0.4, 0.4);
    
    color += phongContribForLight(k_d, k_s, alpha, p, eye,
                                  light1Pos,
                                  light1Intensity);
    
    vec3 light2Pos = rot*vec3(2.0, 2.0, 2.0);
    vec3 light2Intensity = vec3(0.4, 0.4, 0.4);
    
    color += phongContribForLight(k_d, k_s, alpha, p, eye,
                                  light2Pos,
                                  light2Intensity);    
    return color;
}


void main() {
   mat3 rot = rotateY(Time);
   vec3 dir = rot*rayDirection(45.0, WindowSize.xy, gl_FragCoord.xy);
   vec3 eye = rot*vec3(0.0, 0.0, 13.0);
   vec2 res = shortestDistanceToSurface(eye, dir, 
                                        MIN_DIST, MAX_DIST);
   float d = res.x;
   float m = res.y;
    
   if (d > MAX_DIST - EPSILON) {
      // Didn't hit anything
      fragColor = vec4(0.0, 0.0, 0.0, 0.0);
      return;
   }

   // The closest point on the surface to the eyepoint along the view ray
   vec3 p = eye + d * dir;
    
   vec3 K_a = vec3(0.2, 0.2, 0.2);
   vec3 K_d = vec3(0.2, 0.2, 0.2);
   vec3 K_s = vec3(1.0, 1.0, 1.0);

   if(m > 2.5) {
      K_a = vec3(1.0, 1.0, 1.0);
      K_d = vec3(0.2, 0.2, 0.2);
      K_s = vec3(1.0, 1.0, 1.0);
   } else if(m > 1.5) {
      K_a = vec3(0.91, 0.75, 0.67);
      K_d = vec3(0.5, 0.2, 0.2);
      K_s = vec3(0.3, 0.3, 0.3);
   }
   float shininess = 10.0;
    
   vec3 color = phongIllumination(K_a, K_d, K_s, shininess, p, eye, rot);
   
   fragColor = vec4(color, 1.0);   
}
