package dbft2

type DBFT2Context interface {
	GetView() int
	GetHeight() int
	GetBlockTime() int
	GetNumberOfNodes() int
}

type DBFT2ContextService struct {
	view          int
	height        int
	blockTime     int
	numberOfNodes int
}

func NewDBFT2Context(view int, height int, blockTime int, numberOfNodes int) DBFT2Context {
	return &DBFT2ContextService{
		view,
		height,
		blockTime,
		numberOfNodes,
	}
}

func (d *DBFT2ContextService) GetView() int {
	return d.view
}

func (d *DBFT2ContextService) GetHeight() int {
	return d.height
}

func (d *DBFT2ContextService) GetBlockTime() int {
	return d.blockTime
}

func (d *DBFT2ContextService) GetNumberOfNodes() int {
	return d.GetNumberOfNodes()
}
