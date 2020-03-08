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

vx_node vxGenEdgeKNode(vx_graph graph,
                           vx_image inputNeutralEdgeMask,
                           vx_image inputContoneK,
                           vx_image inputRenderedK,
                           vx_uint8 threshLow,
                           vx_uint8 threshHigh,
                           vx_image outputK)
{

    vx_uint32 i;
    vx_node node = 0;
    //get the graph context
    vx_context context = vxGetContext((vx_reference)graph);
    vx_status status = VX_SUCCESS;

    vx_scalar sThreshLow = vxCreateScalar(context, VX_TYPE_UINT8, &threshLow);
    vx_scalar sThreshHigh = vxCreateScalar(context, VX_TYPE_UINT8, &threshHigh);

    //! Retrieving the calculate gradient kernel by name
    vx_kernel kernel = vxGetKernelByName(context, VX_KERNEL_NAME_USER_GENEDGEK);
    if (kernel)
    {
        node = vxCreateGenericNode(graph, kernel);
        if (node)
        {
            vx_status statuses[6];
            statuses[0] = vxSetParameterByIndex(node, 0, (vx_reference)inputNeutralEdgeMask);
            statuses[1] = vxSetParameterByIndex(node, 1, (vx_reference)inputContoneK);
            statuses[2] = vxSetParameterByIndex(node, 2, (vx_reference)inputRenderedK);
            statuses[3] = vxSetParameterByIndex(node, 3, (vx_reference)sThreshLow);
            statuses[4] = vxSetParameterByIndex(node, 4, (vx_reference)sThreshHigh);
            statuses[5] = vxSetParameterByIndex(node, 5, (vx_reference)outputK);

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

    vxReleaseScalar(&sThreshLow);
    vxReleaseScalar(&sThreshHigh);

    return node;
}
