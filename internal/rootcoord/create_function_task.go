package rootcoord

import (
	"context"

	"github.com/milvus-io/milvus/api/commonpb"

	"github.com/milvus-io/milvus/api/milvuspb"
)

type createFunctionTask struct {
	baseTaskV2
	Req *milvuspb.CreateFunctionRequest
}

func (t *createFunctionTask) Prepare(ctx context.Context) error {
	if err := CheckMsgType(t.Req.GetBase().GetMsgType(), commonpb.MsgType_CreateFunction); err != nil {
		return err
	}
	return nil
}

func (t *createFunctionTask) Execute(ctx context.Context) error {
	// create alias is atomic enough.
	return t.core.meta.CreateFunction(ctx, t.Req.GetFunctionName(), t.Req.GetWatFile(), t.Req.GetArgTypes(), t.GetTs())
}
