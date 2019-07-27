package timing

import (
	"fmt"
	"math"
)

const infiniteTime = 1000000000.0

type Timer interface {
	Reset()
	ElapsedTime() float64
	RemainingTime() float64
	Expired() bool
	String() string
}

type TimerService struct {
	clock     Clock
	myTime    float64
	name      string
	countdown float64
}

func NewTimer(name string, clock Clock, countdown float64) Timer {
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
	return t.clock.GetTime() - t.myTime
}

func (t *TimerService) RemainingTime() float64 {
	if t.countdown >= 0.0 {
		return math.Max(0.0, t.countdown - t.ElapsedTime())
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
