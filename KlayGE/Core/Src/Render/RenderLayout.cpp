// RenderLayout.cpp
// KlayGE 渲染流布局类 实现文件
// Ver 3.2.0
// 版权所有(C) 龚敏敏, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// 初次建立 (2006.1.9)
//
// 修改记录
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/MapVector.hpp>

#include <KlayGE/RenderLayout.hpp>

namespace KlayGE
{
	class NullRenderLayout : public RenderLayout
	{
	public:
		NullRenderLayout()
		{
		}
	};

	RenderLayout::RenderLayout()
			: topo_type_(TT_PointList),
				index_format_(EF_Unknown),
				dummy_num_vertices_(0)
	{
		vertex_streams_.reserve(4);
	}

	RenderLayout::~RenderLayout()
	{
	}

	RenderLayoutPtr RenderLayout::NullObject()
	{
		static RenderLayoutPtr obj = MakeSharedPtr<NullRenderLayout>();
		return obj;
	}

	uint32_t RenderLayout::NumVertices() const
	{
		if (vertex_streams_.empty())
		{
			return dummy_num_vertices_;
		}
		else
		{
			return vertex_streams_[0].stream->Size() / vertex_streams_[0].vertex_size;
		}
	}

	void RenderLayout::BindVertexStream(GraphicsBufferPtr const & buffer, vertex_elements_type const & vet,
		stream_type type, uint32_t freq)
	{
		BOOST_ASSERT(buffer);

		uint32_t size = 0;
		for (size_t i = 0; i < vet.size(); ++ i)
		{
			size += vet[i].element_size();
		}

		if (ST_Geometry == type)
		{
			for (size_t i = 0; i < vertex_streams_.size(); ++ i)
			{
				if (vertex_streams_[i].format == vet)
				{
					vertex_streams_[i].stream = buffer;
					vertex_streams_[i].vertex_size = size;
					vertex_streams_[i].type = type;
					vertex_streams_[i].freq = freq;
					return;
				}
			}

			StreamUnit vs;
			vs.stream = buffer;
			vs.format = vet;
			vs.vertex_size = size;
			vs.type = type;
			vs.freq = freq;
			vertex_streams_.push_back(vs);
		}
		else
		{
			instance_stream_.stream = buffer;
			instance_stream_.format = vet;
			instance_stream_.vertex_size = size;
			instance_stream_.type = type;
			instance_stream_.freq = freq;
		}
	}

	void RenderLayout::BindDummyVertexStream(uint32_t num_vertices)
	{
		dummy_num_vertices_ = num_vertices;
	}

	bool RenderLayout::UseIndices() const
	{
		return this->NumIndices() != 0;
	}

	uint32_t RenderLayout::NumIndices() const
	{
		if (index_stream_)
		{
			return index_stream_->Size() / NumFormatBytes(index_format_);
		}
		else
		{
			return 0;
		}
	}

	void RenderLayout::BindIndexStream(GraphicsBufferPtr buffer, ElementFormat format)
	{
		BOOST_ASSERT((EF_R16UI == format) || (EF_R32UI == format));

		index_stream_ = buffer;
		index_format_ = format;
	}

	GraphicsBufferPtr const & RenderLayout::GetIndexStream() const
	{
		BOOST_ASSERT(index_stream_);
		return index_stream_;
	}

	GraphicsBufferPtr const & RenderLayout::InstanceStream() const
	{
		return instance_stream_.stream;
	}

	uint32_t RenderLayout::NumInstance() const
	{
		return vertex_streams_[0].freq;
	}

	void RenderLayout::ExpandInstance(GraphicsBufferPtr& hint, uint32_t inst_no) const
	{
		BOOST_ASSERT(instance_stream_.stream);
		BOOST_ASSERT(inst_no < this->NumInstance());

		RenderFactory& rf = Context::Instance().RenderFactoryInstance();

		uint32_t const num_vertices = this->NumVertices();

		if (!hint)
		{
			hint = rf.MakeVertexBuffer(BU_Dynamic, EAH_GPU_Read, NULL);
		}

		std::vector<uint8_t> instance_buffer(instance_stream_.stream->Size());
		{
			GraphicsBuffer::Mapper mapper(*instance_stream_.stream, BA_Read_Only);
			std::copy(mapper.Pointer<uint8_t>(), mapper.Pointer<uint8_t>() + instance_stream_.stream->Size(),
				instance_buffer.begin());
		}
		GraphicsBufferPtr hint_sys_mem = rf.MakeVertexBuffer(BU_Static, EAH_CPU_Write, NULL);
		hint->Resize(instance_stream_.vertex_size * num_vertices);
		GraphicsBuffer::Mapper dst_mapper(*hint_sys_mem, BA_Write_Only);

		for (uint32_t i = 0; i < num_vertices; ++ i)
		{
			std::copy(&instance_buffer[0] + inst_no * instance_stream_.vertex_size,
				&instance_buffer[0] + (inst_no + 1) * instance_stream_.vertex_size,
				dst_mapper.Pointer<uint8_t>() + i * instance_stream_.vertex_size);
		}

		hint_sys_mem->CopyToBuffer(*hint);
	}
}
