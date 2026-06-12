#version 330 core
out vec4 FragColor;
in vec4 ColorOut;
in vec2 vUv;

uniform sampler2D uTexture;
uniform bool uUseTexture;
uniform bool uMono; // FreeType glyph mi?

void main()
{
    if (uUseTexture)
    {
        if (uMono)
        {
            float alpha = texture(uTexture, vUv).r;
            FragColor = vec4(ColorOut.rgb, alpha * ColorOut.a);
        }
        else
        {
            vec4 t = texture(uTexture, vUv) * ColorOut;
            FragColor = t;
        }
    }
    else
    {
        FragColor = ColorOut;
    }
}
