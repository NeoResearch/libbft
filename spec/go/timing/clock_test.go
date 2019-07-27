package timing

import (
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestClock(t *testing.T) {
	name := "teste"
	clock := NewClock(name)
	assert.NotNil(t, clock)
	assert.Contains(t, clock.String(), name)
}
