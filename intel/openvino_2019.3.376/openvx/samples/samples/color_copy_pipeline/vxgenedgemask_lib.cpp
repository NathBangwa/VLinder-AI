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
#include "vx_user_pipeline_nodes.h"

vx_node vxGenEdgeMaskNode(vx_graph graph,
                          vx_image input,
                          vx_image output)
{

    vx_uint32 i;
    vx_node node = 0;
    //get the graph context
    vx_context context = vxGetContext((vx_reference)graph);
    vx_status status = VX_SUCCESS;

    //! Retrieving the calculate gradient kernel by name
    vx_kernel kernel = vxGetKernelByName(context, VX_KERNEL_NAME_USER_GENEDGEMASK);
    if (kernel)
    {
        node = vxCreateGenericNode(graph, kernel);
        if (node)
        {
            vx_status statuses[2];
            statuses[0] = vxSetParameterByIndex(node, 0, (vx_reference)input);
            statuses[1] = vxSetParameterByIndex(node, 1, (vx_reference)output);

            for (i = 0; i < sizeof(statuses)/sizeof(statuses[0]); i++)
            {
                if (statuses[i] != VX_SUCCESS)
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                    vxReleaseNode(&node);
                    vxReleaseKernel(&kernel);
                    node = 0;
                    kernel = 0;
                    break;
                }
            }
        }
        else
        {
            vxReleaseKernel(&kernel);
        }
    }
    return node;
}
