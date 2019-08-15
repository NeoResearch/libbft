package timing

import (
	"fmt"
	"time"
)

type Clock interface {
	GetTime() time.Time
	String() string
}

type ClockService struct {
	name string
}

func NewDefaultClock() Clock {
	return NewClock("")
}

func NewClock(name string) Clock {
	return &ClockService{
		name,
	}
}

func (c *ClockService) String() string {
	return fmt.Sprintf("Clock {name='%v'}", c.name)
}

func (c *ClockService) GetTime() time.Time {
	return time.Now()
}

func Since(clock Clock) float64 {
	return SinceTime(clock.GetTime())
}

func SinceTime(clock time.Time) float64 {
	return float64(time.Since(clock)) / 1e9
}
