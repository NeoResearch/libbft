package timing

import (
	"fmt"
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestClock(t *testing.T) {
	name := "teste"
	clock := NewClock(name)
	assert.NotNil(t, clock)
	assert.Contains(t, clock.String(), name)

	a := 4
	b := func() int {
		return 10
	}
	c := fmt.Sprintf("a %v %T", a, b)
	fmt.Sprintf("a %v", c)
}
