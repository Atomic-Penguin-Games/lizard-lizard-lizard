#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Flash effect uniforms
uniform float flashIntensity;  // 0.0 = no flash, 1.0 = full white flash
uniform vec3 flashColor;       // Color of the flash (usually white)

// Output fragment color
out vec4 finalColor;

void main()
{
    // Sample the texture
    vec4 texelColor = texture(texture0, fragTexCoord);
    
    // Apply base tinting
    vec4 baseColor = texelColor * colDiffuse * fragColor;
    
    // Apply flash effect by mixing with flash color
    // When flashIntensity is 0, use original color
    // When flashIntensity is 1, use flash color
    vec3 flashedColor = mix(baseColor.rgb, flashColor, flashIntensity);
    
    // Preserve original alpha
    finalColor = vec4(flashedColor, baseColor.a);
}
