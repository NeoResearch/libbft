package timing

type TimerDelayable interface {
	GetTimer() Timer
	String() string
}

type TimerDelayableService struct {
	timer Timer
}

func (t *TimerDelayableService) GetTimer() Timer {
	return t.timer
}

func NewTimerDelayable(name string, clock Clock, countdown float64) TimerDelayable {
	return &TimerDelayableService{
		NewTimer(name, clock, countdown),
	}
}

func (t *TimerDelayableService) String() string {
	return t.timer.String()
}

