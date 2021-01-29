#pragma once
#include "parrot/core/Core.h"

#include <cstdint>
#include <string>
#include <vector>

namespace parrot {

    enum class ShaderDataType : uint8_t {
        None = 0,
        Float   ,
        Float2  ,
        Float3  ,
        Float4  ,
        Mat3    ,
        Mat4    ,
        Int     ,
        Int2    ,
        Int3    ,
        Int4    ,
        Bool    
    };

    static uint32_t getShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float : return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Mat3  : return 4 * 3 * 3;
            case ShaderDataType::Mat4  : return 4 * 4 * 4;
            case ShaderDataType::Int   : return 4;
            case ShaderDataType::Int2  : return 4 * 2;
            case ShaderDataType::Int3  : return 4 * 3;
            case ShaderDataType::Int4  : return 4 * 4;
            case ShaderDataType::Bool  : return 1;
        }

        PR_CORE_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }

    struct BufferElement {
        ShaderDataType type;
        std::string    name;
        uint32_t       size;
        uint32_t       offset;
        bool           normalized;

        BufferElement() : type(ShaderDataType::None), name(), size(0), offset(0), normalized(false) {} 

        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : type(type), name(name), size(getShaderDataTypeSize(type)), offset(0), normalized(normalized) {
        }

        uint32_t getComponentCount() const {
            switch (type) {
                case ShaderDataType::Float : return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3  : return 3 * 3;
                case ShaderDataType::Mat4  : return 4 * 4;
                case ShaderDataType::Int   : return 1;
                case ShaderDataType::Int2  : return 2;
                case ShaderDataType::Int3  : return 3;
                case ShaderDataType::Int4  : return 4;
                case ShaderDataType::Bool  : return 1;
            }

            PR_CORE_ASSERT(false, "Unknown ShaderDataType");
            return 0;
        }
    };

    class BufferLayout {
    public:
        BufferLayout() : m_element_list(), m_stride(0) {}
        BufferLayout(const std::initializer_list<BufferElement> elements) : m_element_list(elements) {
            uint32_t offset = 0;
            m_stride = 0;
            for (auto& element : m_element_list) {
                element.offset = offset;
                offset   += element.size;
                m_stride += element.size;
            }
        }

        inline uint32_t getStride() const { return m_stride; }
        inline const std::vector<BufferElement>& getElementList() const { return m_element_list; }

        std::vector<BufferElement>::iterator       begin()       { return m_element_list.begin(); }
        std::vector<BufferElement>::iterator       end  ()       { return m_element_list.end  (); }
        std::vector<BufferElement>::const_iterator begin() const { return m_element_list.begin(); }
        std::vector<BufferElement>::const_iterator end  () const { return m_element_list.end  (); }

    private:
        std::vector<BufferElement> m_element_list;
        uint32_t                   m_stride = 0;
    };

    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void                bind     () const = 0;
        virtual void                unbind   () const = 0;
        virtual void                setLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& getLayout() const = 0;

        static VertexBuffer* create(float* vertices, uint32_t size);
    };

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void     bind()     const = 0;
        virtual void     unbind()   const = 0;
        virtual uint32_t getCount() const = 0;

        static IndexBuffer* create(uint32_t* indices, uint32_t count);
    };

}