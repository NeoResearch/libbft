package timing

import (
	"fmt"
	"math"
	"time"
)

const infiniteTime = 1000000000.0

type Timer interface {
	// get / set
	GetCountdown() float64
	// methods
	Reset()
	ElapsedTime() float64
	RemainingTime() float64
	Expired() bool
	InitDefault() Timer
	Init(countdown float64) Timer

	String() string
}

type TimerService struct {
	clock     Clock
	myTime    time.Time
	name      string
	countdown float64
}

func NewDefaultTimer() Timer {
	return NewTimer("", nil, -1.0)
}

func NewTimerName(name string) Timer {
	return NewTimer(name, nil, -1.0)
}

func NewTimer(name string, clock Clock, countdown float64) Timer {
	if clock == nil {
		clock = NewDefaultClock()
	}
	return &TimerService{
		clock,
		clock.GetTime(),
		name,
		countdown,
	}
}

func (t *TimerService) Reset() {
	t.myTime = t.clock.GetTime()
}

func (t *TimerService) ElapsedTime() float64 {
	return SinceTime(t.myTime)
}

func (t *TimerService) RemainingTime() float64 {
	if t.GetCountdown() >= 0.0 {
		return math.Max(0.0, t.GetCountdown()-t.ElapsedTime())
	} else {
		return infiniteTime
	}
}

func (t *TimerService) Expired() bool {
	return t.RemainingTime() == 0.0
}

func (t *TimerService) String() string {
	return fmt.Sprintf("Timer {name='%v'}", t.name)
}

func (t *TimerService) GetCountdown() float64 {
	return t.countdown
}

func (t *TimerService) Init(countdown float64) Timer {
	t.countdown = countdown
	if t.clock == nil {
		t.clock = NewDefaultClock()
	}
	t.myTime = t.clock.GetTime()
	return t
}

func (t *TimerService) InitDefault() Timer {
	return t.Init(-1)
}
