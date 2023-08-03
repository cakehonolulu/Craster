#include <SDL2/SDL.h>
#include <craster.h>

#define WIDTH   640
#define HEIGHT  480

void MultiplyMatrixVector(vec3 *i, vec3 *o, mat4x4 *m)
{
    o->x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
	o->y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
	o->z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];
	float w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];

	if (w != 0.0f)
	{
		o->x /= w; o->y /= w; o->z /= w;
	}
}

void draw_tri(triangle tri, SDL_Renderer *renderer)
{
    SDL_RenderDrawLine(renderer, tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y);
	SDL_RenderDrawLine(renderer, tri.p[1].x, tri.p[1].y, tri.p[2].x, tri.p[2].y);
	SDL_RenderDrawLine(renderer, tri.p[2].x, tri.p[2].y, tri.p[0].x, tri.p[0].y);
}

int main(char argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("Craster",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window,
    -1, SDL_RENDERER_PRESENTVSYNC);

    triangle mesh[] =
    {
        // SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f }

	};

    triangle mesh2[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f, 
        0.0f, 0.5f, 0.0f
    };

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = WIDTH / HEIGHT;
    float fFovRad = 1.0f / tan(fFov * 0.5f / 180.0f * 3.14159f);

    mat4x4 matProj = {0};

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;
    
    float fTheta, elapsedMS;

    unsigned char currentcol = 0;

    float frequency = 0.1f;

    unsigned char red, green, blue;

    vec3 vCamera = {0};

    unsigned char frames = 0;

    while (1)
    {
        unsigned long long start = SDL_GetPerformanceCounter();

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) exit(0);
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        mat4x4 matRotZ = {0}, matRotX = {0};
        fTheta += 1.0f * 0.5;

        matRotZ.m[0][0] = cos(fTheta);
        matRotZ.m[0][1] = sin(fTheta);
        matRotZ.m[1][0] = -sin(fTheta);
        matRotZ.m[1][1] = cos(fTheta);
        matRotZ.m[2][2] = 1;
        matRotZ.m[3][3] = 1;

        matRotX.m[0][0] = cos(fTheta * 0.1f);
        matRotX.m[0][2] = -sin(fTheta * 0.1f);
        matRotX.m[1][1] = 1;
        matRotX.m[2][0] = sin(fTheta * 0.1f);
        matRotX.m[2][2] = cos(fTheta * 0.1f);
        matRotX.m[3][3] = 1;

        red   = sin(frequency*currentcol + 0) * 127 + 128;
        green = sin(frequency*currentcol + 2) * 127 + 128;
        blue  = sin(frequency*currentcol + 4) * 127 + 128;
        
        if (currentcol == 127)
        {
            currentcol = 0;
        }
        else
        {
            currentcol++;
        }

        for (int i = 0; i < (sizeof(mesh) / sizeof(mesh[0])); i++)
        {
            triangle triProjected = {0}, triTranslated = {0}, triRotatedZ = {0}, triRotatedZX = {0};

            MultiplyMatrixVector(&mesh[i].p[0], &triRotatedZ.p[0], &matRotZ);
            MultiplyMatrixVector(&mesh[i].p[1], &triRotatedZ.p[1], &matRotZ);
            MultiplyMatrixVector(&mesh[i].p[2], &triRotatedZ.p[2], &matRotZ);

            MultiplyMatrixVector(&mesh[i].p[0], &triRotatedZX.p[0], &matRotX);
            MultiplyMatrixVector(&mesh[i].p[1], &triRotatedZX.p[1], &matRotX);
            MultiplyMatrixVector(&mesh[i].p[2], &triRotatedZX.p[2], &matRotX);


            triTranslated = triRotatedZX;

            triTranslated.p[0].z = triRotatedZX.p[0].z + 5.0f;
            triTranslated.p[1].z = triRotatedZX.p[1].z + 5.0f;
            triTranslated.p[2].z = triRotatedZX.p[2].z + 5.0f;

            vec3 normal, line1, line2;

            line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
            line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
            line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

            line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
            line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
            line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

            normal.x = line1.y * line2.z - line1.z * line2.y;
            normal.y = line1.z * line2.x - line1.x * line2.z;
            normal.z = line1.x * line2.y - line1.y * line2.x;

            float l = sqrtf(normal.x*normal.x + normal.y * normal.y + normal.z * normal.z);
            normal.x /= l;
            normal.y /= l;
            normal.z /= l;

            //if (normal.z < 0)
            if (normal.x * (triTranslated.p[0].x - vCamera.x) + 
                normal.y * (triTranslated.p[0].y - vCamera.y) +
                normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
            {

                vec3 light_direction = { 0.0f, 0.0f, -1.0 };

                float l = sqrtf(light_direction.x*light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
                light_direction.x /= l;
                light_direction.y /= l;
                light_direction.z /= l;

                float dp = normal.x * light_direction.x + normal.y * light_direction.y +  normal.z * light_direction.z;
                
#ifndef TRIANG
                MultiplyMatrixVector(&triTranslated.p[0], &triProjected.p[0], &matProj);
                MultiplyMatrixVector(&triTranslated.p[1], &triProjected.p[1], &matProj);
                MultiplyMatrixVector(&triTranslated.p[2], &triProjected.p[2], &matProj);
#else
                MultiplyMatrixVector(&mesh2->p[0], &triProjected.p[0], &matProj);
                MultiplyMatrixVector(&mesh2->p[1], &triProjected.p[1], &matProj);
                MultiplyMatrixVector(&mesh2->p[2], &triProjected.p[2], &matProj);
#endif

                triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
                triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
                triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

                triProjected.p[0].x *= 0.5f * WIDTH;
                triProjected.p[0].y *= 0.5f * HEIGHT;
                triProjected.p[1].x *= 0.5f * WIDTH;
                triProjected.p[1].y *= 0.5f * HEIGHT;
                triProjected.p[2].x *= 0.5f * WIDTH;
                triProjected.p[2].y *= 0.5f * HEIGHT;

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                SDL_FPoint p1, p2, p3;
                SDL_Color c1, c2, c3;
                
                c1.r = 255;
                c1.g = 0;
                c1.b = 0;
                c1.a = 255;

                c2.r = 0;
                c2.g = 0;
                c2.b = 255;
                c2.a = 255;

                c3.r = 0;
                c3.g = 255;
                c3.b = 0;
                c3.a = 255;

                p1.x = triProjected.p[0].x;
                p1.y = triProjected.p[0].y;

                p2.x = triProjected.p[1].x;
                p2.y = triProjected.p[1].y;

                p3.x = triProjected.p[2].x;
                p3.y = triProjected.p[2].y;

                SDL_Vertex vertex[3] = {
                    { p1, c1, 0 },
                    { p2, c2, 0 },
                    { p3, c3, 0 }
                };

                SDL_RenderGeometry( renderer, NULL, &vertex[0], 3, NULL, 0 );

#ifndef NO_WIREFRAME
                draw_tri(triProjected, renderer);
#endif
            }
        }


        SDL_RenderPresent(renderer);

        unsigned long long end = SDL_GetPerformanceCounter();

	    elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        char string[64];
        snprintf(string, 64, "Craster | %.02f FPS, (%.04f ms/f)", ((1/elapsedMS) * 1000), elapsedMS);

        if (16.666f - elapsedMS > 0)
        {
            SDL_Delay(floor(16.666f - elapsedMS));
        }

        if (frames >= 60)
        {
            frames = 0;
            SDL_SetWindowTitle(window, string);
        }

        frames++;
    }

    return 0;
}
