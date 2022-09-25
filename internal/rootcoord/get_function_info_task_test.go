package rootcoord

import (
	"context"
	"github.com/milvus-io/milvus/internal/proto/rootcoordpb"
	"testing"

	"github.com/milvus-io/milvus/api/commonpb"
	"github.com/stretchr/testify/assert"
)

func Test_getFunctionInfoTask_Prepare(t *testing.T) {
	t.Run("invalid msg type", func(t *testing.T) {
		task := &getFunctionInfoTask{
			Req: &rootcoordpb.GetFunctionInfoRequest{
				Base: &commonpb.MsgBase{
					MsgType: commonpb.MsgType_CreateFunction,
				},
			},
		}
		err := task.Prepare(context.Background())
		assert.Error(t, err)
	})

	t.Run("normal case", func(t *testing.T) {
		task := &getFunctionInfoTask{
			Req: &rootcoordpb.GetFunctionInfoRequest{
				Base: &commonpb.MsgBase{
					MsgType: commonpb.MsgType_GetFunctionInfo,
				},
			},
		}
		err := task.Prepare(context.Background())
		assert.NoError(t, err)
	})
}

func Test_getFunctionInfoTask_Execute(t *testing.T) {
	t.Run("get function", func(t *testing.T) {
		core := newTestCore(withInvalidMeta())
		task := &getFunctionInfoTask{
			baseTaskV2: baseTaskV2{core: core},
			Req: &rootcoordpb.GetFunctionInfoRequest{
				Base: &commonpb.MsgBase{
					MsgType: commonpb.MsgType_GetFunctionInfo,
				},
				FunctionName: "test",
			},
		}
		err := task.Execute(context.Background())
		assert.Error(t, err)
	})
}
