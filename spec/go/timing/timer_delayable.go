package timing

type TimerDelayable interface {
	Reset()
	ElapsedTime() float64
	RemainingTime() float64
	Expired() bool

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

func (t *TimerDelayableService) Reset() {
	t.GetTimer().Reset()
}

func (t *TimerDelayableService) ElapsedTime() float64 {
	return t.GetTimer().ElapsedTime()
}

func (t *TimerDelayableService) RemainingTime() float64 {
	return t.GetTimer().RemainingTime()
}

func (t *TimerDelayableService) Expired() bool {
	return t.GetTimer().Expired()
}
