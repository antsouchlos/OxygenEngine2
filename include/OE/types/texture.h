#ifndef OE_TEXTURE_H
#define OE_TEXTURE_H

#include <OE/types/base_types.h>
#include <atomic>

/* OE_Texture represents a simple texture.
 * It can be assigned a camera or an image or video as the source
 * Or even arbitrary data
 */
class OE_Texture : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase {

    friend class CSL_Interpreter;

public:
    static std::atomic<std::size_t> current_id;
    const std::size_t               id;

    OE_Texture();
    ~OE_Texture();

    std::string to_str() const;

    //    protected:
    int         source;
    std::string path;
    std::size_t camera;
    void*       custom_data;

private:
};

#endif
