/*
        Copyright 2019 Intel Corporation.
        This software and the related documents are Intel copyrighted materials,
        and your use of them is governed by the express license under which they
        were provided to you (End User License Agreement for the Intel(R) Software
        Development Products (Version May 2017)). Unless the License provides
        otherwise, you may not use, modify, copy, publish, distribute, disclose or
        transmit this software or the related documents without Intel's prior
        written permission.

        This software and the related documents are provided as is, with no
        express or implied warranties, other than those that are expressly
        stated in the License.
*/

#include <iostream>
#include <intel/vx_samples/helper.hpp>
#include "vx_user_pipeline_nodes.h"

extern vx_kernel sharedKernel;

#if !INTEL_SAMPLE_OPENCL_BUG_CVS_1633_KERNEL_BY_NAME
vx_node vxSymm7x7OpenCLNode(vx_graph graph, vx_image input, vx_image output, const char* kernelName)
#else
vx_node vxSymm7x7OpenCLNode(vx_graph graph, vx_image input, vx_image output, vx_enum kernelEnum)
#endif
{
    vx_uint32 i;
    vx_node node = 0;
    vx_context context = vxGetContext((vx_reference)graph);
    vx_status status = VX_SUCCESS;

    //! Retrieving the census transform Node by name
    #if !INTEL_SAMPLE_OPENCL_BUG_CVS_1633_KERNEL_BY_NAME
    vx_kernel kernel = vxGetKernelByName(context, kernelName);
    #else
    vx_kernel kernel = vxGetKernelByEnum(context, kernelEnum);
    #endif

    CHECK_VX_OBJ(kernel);
    if (kernel)
    {
        node = vxCreateGenericNode(graph, kernel);
        CHECK_VX_OBJ(node);
        if (node)
        {
            vx_status statuses[2];
            CHECK_VX_OBJ(input);
            statuses[0] = vxSetParameterByIndex(node, 0, (vx_reference)input);
            CHECK_VX_STATUS(statuses[0]);
            CHECK_VX_OBJ(output);
            statuses[1] = vxSetParameterByIndex(node, 1, (vx_reference)output);
            CHECK_VX_STATUS(statuses[1]);
            for (i = 0; i < sizeof(statuses)/sizeof(statuses[0]); i++)
            {
                if (statuses[i] != VX_SUCCESS)
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                    vxReleaseNode(&node);
                    vxReleaseKernel(&kernel);
                    node = 0;
                    kernel = 0;
                    // TODO: forward this message to OpenVX message callback
                    std::cerr << "[ WARNING ] Cannot set parameter by index " << i << " in vxSymm7x7OpenCLNode\n";
                    break;
                }
            }
        }
        else
        {
            // TODO: forward this message to OpenVX message callback
            std::cerr << "[ WARNING ] vxCreateGenericNode(graph, kernel) for OpenCLNode failed\n";
            vxReleaseKernel(&kernel);
        }
    }
    else
    {
        // TODO: forward this message to OpenVX message callback
        std::cerr << "[ WARNING ] vxGetKernelByName(context, VX_KERNEL_NAME_USER_SYMM7X7_OPENCL) failed\n";
    }
    return node;
}


vx_node vxSymm7x7OpenCLNode(vx_graph graph, vx_image input, vx_image output)
{
    #if !INTEL_SAMPLE_OPENCL_BUG_CVS_1633_KERNEL_BY_NAME
    return vxSymm7x7OpenCLNode(graph, input, output, VX_KERNEL_NAME_USER_SYMM7X7_OPENCL);
    #else
    return vxSymm7x7OpenCLNode(graph, input, output, VX_KERNEL_USER_SYMM7X7_OPENCL);
    #endif
}

vx_node vxSymm7x7OpenCLTiledNode(vx_graph graph, vx_image input, vx_image output)
{
    #if !INTEL_SAMPLE_OPENCL_BUG_CVS_1633_KERNEL_BY_NAME
    return vxSymm7x7OpenCLNode(graph, input, output, VX_KERNEL_NAME_USER_SYMM7X7_OPENCL_TILED);
    #else
    return vxSymm7x7OpenCLNode(graph, input, output, VX_KERNEL_USER_SYMM7X7_OPENCL_TILED);
    #endif
}

