uniform mediump sampler2D diffuse;

varying highp vec3 vecTransformed;
varying highp vec2 vecUVCoords;

void main(){
	highp vec3 Diffuse = texture2D(diffuse, vecUVCoords).rgb;
	highp vec3 Color = Diffuse;
	Color += Diffuse*0.15;
	highp vec3 vector = normalize(vecTransformed*0.5+0.5);
	gl_FragColor = vec4(vector,1.0);
}