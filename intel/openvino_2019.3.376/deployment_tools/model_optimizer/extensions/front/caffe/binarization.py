#-*-coding:utf-8 -*-
"""
 Copyright (c) 2019 Intel Corporation

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
"""
import numpy as np

from extensions.ops.fakequantize import FakeQuantize
from mo.front.common.replacement import FrontReplacementOp
from mo.graph.graph import Node, Graph
from mo.ops.const import Const


class BinarizationToQuantize(FrontReplacementOp):
    """
    Replaces Binarization layer with Quantize.
    """
    op = "Binarization"
    enabled = True

    def replace_op(self, graph: Graph, node: Node):
        in_node_0 = node.in_node(0)

        broadcast = lambda x: np.array([x], dtype=np.float32)
        threshold = Const(graph, {'name': node.id + "/Input_1", "value": broadcast(0)}).create_node()
        in_1 = threshold
        in_2 = threshold
        in_3 = Const(graph, {'name': node.id + "/Input_3", "value": broadcast(-1)}).create_node()
        in_4 = Const(graph, {'name': node.id + "/Input_4", "value": broadcast(+1)}).create_node()
        quant = FakeQuantize(graph, {'name': node.id + "/FakeQuantize_", "levels": 2}).create_node(
            inputs=[in_node_0, in_1, in_2, in_3, in_4])

        return [quant.id]