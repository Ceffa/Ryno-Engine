#include "Sprite.h"
#include "Global.h"
#include "TextureManager.h"

namespace Ryno{

    std::list<Sprite*> Sprite::sprites;

    Sprite::Sprite(){

        sprites.push_back(this);
        scale = glm::vec2(1, 1);
    }
    Sprite::Sprite(const Sprite *s)
    {
        *this = *s;
        sprites.push_back(this);
    }
    Sprite::~Sprite(){
        sprites.remove(this);
    }

    void Sprite::generate_model_matrix()
    {
        F32 deg_angle = -angle * DEG_TO_RAD;
        F32 sn = sin(deg_angle);
        F32 cs = cos(deg_angle);


        F32 new_sc_x = ratio * scale.x / WINDOW_WIDTH;
        F32 new_sc_y =  scale.y / WINDOW_HEIGHT;


        F32 new_x = (position.x - 0.5) * 2;
        F32 new_y = (position.y - 0.5) * 2;

        //Adapt position depending on anchoring point
        switch (anchor_point)
        {
        case TOP_LEFT:
            new_x += new_sc_x;
            new_y -= new_sc_y;
            break;
        case TOP_RIGHT:
            new_x -= new_sc_x;
            new_y -= new_sc_y;
            break;
        case BOTTOM_LEFT:
            new_x += new_sc_x;
            new_y += new_sc_y;
            break;
        case BOTTOM_RIGHT:
            new_x -= new_sc_x;
            new_y += new_sc_y;
            break;
        default:
            break;
        }



        model_matrix = glm::mat3(
            glm::vec3(new_sc_x * cs, new_sc_x * sn, 0),
            glm::vec3(-new_sc_y * sn, new_sc_y * cs, 0),
            glm::vec3(new_x, new_y, 1)
            );
    }

    void Sprite::set_texture(Texture t)
    {
        texture = t;
        restore_image_ratio();

    }

    void Sprite::set_ratio(U32 w, U32 h)
    {
        ratio = w / (F32)h;

    }

    void Sprite::restore_image_ratio(){
        set_ratio(texture.width, texture.height);
    }



}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                