package machine

type MachineId interface {
	// get/set
	GetId() int
	GetAddress() string
}

type MachineIdService struct {
	id      int
	address string
}

func NewMachineId(id int, address string) MachineId {
	return &MachineIdService{
		id:      id,
		address: address,
	}
}

func NewDefaultMachineId() MachineId {
	return NewMachineId(0, "")
}

func (m *MachineIdService) GetId() int {
	return m.id
}

func (m *MachineIdService) GetAddress() string {
	return m.address
}
