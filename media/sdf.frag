uniform sampler2D texture;
uniform vec4 finalcolour;

void main() {
    // retrieve distance from texture
    float mask = texture2D( texture, gl_TexCoord[0].xy).a;

    // use current drawing color
    vec4 clr;
    clr.rgb = finalcolour.rgb;
    // perform simple thresholding
    if( mask < 0.5 )  
        clr.a = 0.0;
    else {
        /*if (mask < 0.9) { // Outline effect
            clr.rgb = vec3(1.0,0.0,1.0);
        }*/
        clr.a = finalcolour.a;
    }


    // do some anti-aliasing
    clr.a *= smoothstep(0.45, 0.55, mask);

    /*if(clr.a < 0.1) {
        vec4 glowTexel = texture2D( texture, gl_TexCoord[0].xy - vec2(0.01, 0.06) ) ;
        vec4 glowc =  vec4(0, 0, 0, 0.5);
        if( glowTexel.a > 0.5 )
            clr = glowc;
    }*/

    if(clr.a < 0.05)
        discard;

    // final color
    gl_FragColor = clr; 
}
