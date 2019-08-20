package main

import (
	"fmt"
	"github.com/NeoResearch/libbft/src/machine"
	"github.com/NeoResearch/libbft/src/single"
	"github.com/NeoResearch/libbft/src/timing"
	"github.com/NeoResearch/libbft/src/util"
	"os"
	"os/exec"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	initial := single.NewState(false, "Initial")
	final := single.NewState(true, "Final")

	initial.AddTransition(single.NewTransaction(final, "after1sec").AddCondition(single.NewCondition("C >= 1", func(t timing.Timer, d single.Param, me int) (bool, error) {
		fmt.Printf("WAITING TRANSITION TO HAPPEN %v s\n", t.ElapsedTime())
		return t.ElapsedTime() >= 3.0, nil
	})))

	myMachine := machine.NewSingleTimerStateMachineClock(timing.NewTimerName("C"))

	err := myMachine.RegisterState(initial)
	check(err)
	err = myMachine.RegisterState(final)
	check(err)

	fmt.Printf("Machine => %v\n", myMachine)

	fmt.Println("BEFORE RUN, WILL PRINT AS GRAPHVIZ!")
	graphviz := myMachine.StringFormat(util.GraphivizFormat)
	fmt.Printf("%v\n", graphviz)

	myMachine.SetWatchdog(5)
	myMachine.RunDefault()

	fileName := "fgraph_STSM.dot"
	f, err := os.Create(fileName)
	check(err)
	defer f.Close()

	_, err = f.WriteString(fmt.Sprintf("%v\n", graphviz))
	check(err)
	imageName := "fgraph_STSM.png"

	//fmt.Printf("Generating image '%v'\n", imageName)
	//cmd := exec.Command("dot", fmt.Sprintf("-Tpng %v -o %v", fileName, imageName))
	//err = cmd.Run()
	//check(err)
}
