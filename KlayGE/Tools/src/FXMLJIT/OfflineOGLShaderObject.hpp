/**
* @file OfflineOGLShaderObject.hpp
* @author Minmin Gong
*
* @section DESCRIPTION
*
* This source file is part of KlayGE
* For the latest info, see http://www.klayge.org
*
* @section LICENSE
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published
* by the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* You may alternatively use this source under the terms of
* the KlayGE Proprietary License (KPL). You can obtained such a license
* from http://www.klayge.org/licensing/.
*/

#ifndef _OFFLINEOGLSHADEROBJECT_HPP
#define _OFFLINEOGLSHADEROBJECT_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include "OfflineShaderObject.hpp"

namespace KlayGE
{
	namespace Offline
	{
		class OGLShaderObject : public ShaderObject
		{
		public:
			explicit OGLShaderObject(OfflineRenderDeviceCaps const & caps);

			std::string GenHLSLShaderText(ShaderType type, RenderEffect const & effect,
				RenderTechnique const & tech, RenderPass const & pass) const;

			virtual void StreamOut(std::ostream& os, ShaderType type) KLAYGE_OVERRIDE;

			void AttachShader(ShaderType type, RenderEffect const & effect,
				RenderTechnique const & tech, RenderPass const & pass, std::vector<uint32_t> const & shader_desc_ids);
			void AttachShader(ShaderType type, RenderEffect const & effect,
				RenderTechnique const & tech, RenderPass const & pass, ShaderObjectPtr const & shared_so);
			void LinkShaders(RenderEffect const & effect);

		private:
			shared_ptr<array<std::string, ST_NumShaderTypes> > shader_func_names_;
			shared_ptr<array<shared_ptr<std::string>, ST_NumShaderTypes> > glsl_srcs_;
			shared_ptr<array<shared_ptr<std::vector<std::string> >, ST_NumShaderTypes> > pnames_;
			shared_ptr<array<shared_ptr<std::vector<std::string> >, ST_NumShaderTypes> > glsl_res_names_;
			shared_ptr<std::vector<VertexElementUsage> > vs_usages_;
			shared_ptr<std::vector<uint8_t> > vs_usage_indices_;
			shared_ptr<std::vector<std::string> > glsl_vs_attrib_names_;
			int32_t gs_input_type_, gs_output_type_, gs_max_output_vertex_;

			std::vector<tuple<std::string, RenderEffectParameterPtr, RenderEffectParameterPtr, uint32_t> > tex_sampler_binds_;
		};

		typedef shared_ptr<OGLShaderObject> OGLShaderObjectPtr;
	}
}

#endif			// _OFFLINEOGLSHADEROBJECT_HPP
