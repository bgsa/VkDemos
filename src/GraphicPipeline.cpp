#include "GraphicPipeline.h"

namespace VkBootstrap
{
	void GraphicPipeline::createPipelineLayout(const Device* device)
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		VkResult operationResult = vkCreatePipelineLayout(device->logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout);

		if (operationResult != VK_SUCCESS)
			throw std::runtime_error("failed to create pipeline layout: " + VkHelper::getVkResultDescription(operationResult));
	}

	GraphicPipeline::GraphicPipeline(const Device *device, Shader *shader, SwapChain *swapChain, Viewport *viewport, VkPipelineVertexInputStateCreateInfo* vertexInput, VkDescriptorSetLayout* descriptorSetLayout, const std::vector<VkDescriptorSet>& descriptorSets )
	{
		this->device = device->logicalDevice;
		this->descriptorSetLayout = descriptorSetLayout;
		this->descriptorSets = descriptorSets;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;          // Optional
		multisampling.pSampleMask = nullptr;            // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE;      // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional
		
		createPipelineLayout(device);

		viewportState = viewport->getViewportState();

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.pNext = nullptr;
		dynamicStateInfo.flags = 0;
		dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicStateInfo.pDynamicStates = dynamicStates.data();

		//create graphic pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shader->getPipelinesShaderStageInfo();
		pipelineInfo.pVertexInputState = vertexInput;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pRasterizationState = Rasterizer::getState();
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicStateInfo;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = swapChain->renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1;              // Optional
		pipelineInfo.pViewportState = viewportState;
		pipelineInfo.pTessellationState = nullptr;

		VkResult operationResult = vkCreateGraphicsPipelines(device->logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicPipeline);

		if (operationResult != VK_SUCCESS)
			throw std::runtime_error("failed to create graphics pipeline!");
	}

	GraphicPipeline::~GraphicPipeline()
	{
		if (graphicPipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(device, graphicPipeline, nullptr);
			graphicPipeline = VK_NULL_HANDLE;
		}

		if (pipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
			pipelineLayout = VK_NULL_HANDLE;
		}

		delete viewportState;
		viewportState = nullptr;
	}

}
