package bot

import (
	"fmt"
	"math/rand"
	"time"

	"github.com/go-rod/rod"
	"github.com/go-rod/rod/lib/proto"
)

const WaitDuration = time.Second * 10

var Replies = []string{
	"Hello, <br><br>I hope you will cope harder.",
	"Dear user! <br><br>I am proud to inform you, that I don't care. <br><br>Best regards.",
	"Dear user! <br><br>Please, stop spamming",
	"Ha-ha funny",
	"I swear to god, write something one more time and I will ban you",
	"dizhablarser",
	"Skill issue",
	"RIP Bozo",
	"Try harder",
	"I bet your main category is OSINT",
	"You better return watching cocomelon",
	"L + Ratio + You fell off + Cope",
	"EBIN :DDDDDD",
}

func Visit(login, password, url string) error {
	browser := rod.New()

	err := browser.Connect()
	if err != nil {
		fmt.Printf("error on browser.Connect(): %v\n", err)
		return err
	}
	defer browser.MustClose()

	incognito, err := browser.Incognito()

	if err != nil {
		fmt.Printf("error on browser.Incognito(): %v\n", err)
		return err
	}

	page, err := incognito.Page(proto.TargetCreateTarget{})

	if err != nil {
		fmt.Printf("error on incognito.Page(): %v\n", err)
		page.Close()
		return err
	}

	err = page.Navigate(url)

	if err != nil {
		fmt.Printf("error on page.Navigate(url): %v\n", err)
		page.Close()
		return err
	}

	err = page.WaitStable(time.Millisecond * 250)
	el, err := page.Element("#username")

	if err != nil {
		fmt.Printf("error on page.Element(#username): %v\n", err)
		page.Close()
		return err
	}

	err = el.Input(login)
	el, err = page.Element("#password")

	if err != nil {
		fmt.Printf("error on page.Element(#password): %v\n", err)
		page.Close()
		return err
	}

	err = el.Input(password)
	el, err = page.Element("button[type=submit]")

	if err != nil {
		fmt.Printf("error on page.Element(button[type=submit]): %v\n", err)
		page.Close()
		return err
	}

	el.Click(proto.InputMouseButtonLeft, 1)
	fmt.Println("Clicked on button")

	page = page.Timeout(WaitDuration)
	fmt.Println("Timeout set")

	err = page.WaitStable(time.Millisecond * 250)
	fmt.Println("Content stable")

	err = page.Navigate(url)
	fmt.Println("Navigating to comment")

	if err != nil {
		fmt.Printf("error on page.Navigate(url): %v\n", err)
		<-page.GetContext().Done()
		page.Close()
		return err
	}

	err = page.WaitStable(time.Millisecond * 250)
	el, err = page.Element("textarea")

	if err != nil {
		fmt.Printf("error on page.Element(textarea): %v\n", err)
		<-page.GetContext().Done()
		page.Close()
		return err
	}

	err = el.Input(
		Replies[rand.Intn(len(Replies))] +
			"<br><br>" +
			"Sent from totally secure machine isolated from worldwide web" +
			"<br><br>" +
			"Best, regards" +
			"<br>" +
			"MZh Bo$$",
	)
	el, err = page.Element("button[type=submit]")

	if err != nil {
		fmt.Printf("error on page.Element(button[type=submit]): %v\n", err)
		<-page.GetContext().Done()
		page.Close()
		return err
	}

	err = el.Click(proto.InputMouseButtonLeft, 1)

	<-page.GetContext().Done()

	fmt.Printf("Browser done, closing...\n")
	return page.Close()
}
