#ifndef __PARSER_H__
#define __PARSER_H__

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include "Type.h"

#include <memory>

class Parser
{
    public :
        struct Buffer
        {
            UINT32 VAO;
            UINT32 VBO;
            UINT32 EBO;

            Buffer();
            Buffer(const UINT32 VAO, const UINT32 VEO, const UINT32 EBO);
        };

        Parser();
        virtual ~Parser();

    public :
        std::shared_ptr<Buffer> GetTriangle();
        std::shared_ptr<Buffer> GetRectangle(); 
};

#endif // __PARSER_H__