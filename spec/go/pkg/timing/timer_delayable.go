package timing

type TimerDelayable interface {
	// get / set
	GetCountdown() float64
	// methods
	Reset()
	ElapsedTime() float64
	RemainingTime() float64
	Expired() bool
	Init(countdown float64) Timer

	GetTimer() Timer
	String() string
}

type TimerDelayableService struct {
	timer Timer
}

func NewTimerDelayable(name string, clock Clock, countdown float64) TimerDelayable {
	return &TimerDelayableService{
		NewTimer(name, clock, countdown),
	}
}

func (t *TimerDelayableService) String() string {
	return t.String()
}

func (t *TimerDelayableService) Reset() {
	t.Reset()
}

func (t *TimerDelayableService) ElapsedTime() float64 {
	return t.ElapsedTime()
}

func (t *TimerDelayableService) RemainingTime() float64 {
	return t.RemainingTime()
}

func (t *TimerDelayableService) Expired() bool {
	return t.Expired()
}

func (t *TimerDelayableService) GetCountdown() float64 {
	return t.GetTimer().GetCountdown()
}

func (t *TimerDelayableService) Init(countdown float64) Timer {
	return t.GetTimer().Init(countdown)
}

func (t *TimerDelayableService) GetTimer() Timer {
	return t.timer
}