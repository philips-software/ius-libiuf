# Contributing to IUF file IO library SDK

First off all, thank you for considering contributing to IUF file IO library SDK!

Following these guidelines helps to communicate that you respect the time of the developers managing and developing this open source project. In return, they should reciprocate that respect in addressing your issue, assessing changes, and helping you finalize your pull requests.

### Contributing to development

IUF file IO library SDK is a young open source project and we love to receive contributions from our community — you! There are many ways to contribute, from writing examples, improving the documentation, submitting bug reports and feature requests or writing code which can be incorporated into IUF file IO library SDK itself.

# Ground Rules

* Be professional, respectful and considerate in your communication with others.
* As IUF file IO library SDK core-team we have spent considerate effort to make our components testable; please make sure that you have test coverage for your changes as well. Inspiration can be drawn from existing sources.
* Ensure cross-platform compatibility for your changes.
* Create issues for any major changes and enhancements that you wish to make. Discuss things transparently and get core-team feedback.

### Test are not optional

We can't stress this enough. Any bugfix that doesn’t include a test proving the existence of the bug being fixed, may be suspect. Ditto for new features that can’t prove they actually work. We’ve found that test-first development really helps make features better architected and identifies potential edge cases earlier instead of later. Writing tests before the implementation is strongly encouraged.

# Your First Contribution

### Introduction

Submitting your first contribution (or Pull Request) can be scary, but we promise you it will be very rewarding! Information on how to proceed can be found on numerous blog posts and websites. A small selection of beginner friendly tutorials:

**Working on your first Pull Request?** You can learn how from this *free* series [How to Contribute to an Open Source Project on GitHub](https://egghead.io/series/how-to-contribute-to-an-open-source-project-on-github)

**Contributing to open source for the first time can be scary and a little overwhelming.** Perhaps you’re a [Code Newbie](https://codenewbie.org/) or maybe you’ve been coding for a while but haven’t found a project you felt comfortable contributing to. [You can do it! here's how.](https://www.firsttimersonly.com/)

### Full example

Make sure you have all development dependencies installed. See dependecies section in the [README](README.md) document.

#### Preparing your Fork

1. Click ‘Fork’ on Github, creating e.g. yourname/ius-libiuf.
2. Clone your project: git clone git@github.com:yourname/ius-libiuf.
3. ```cd ius-libiuf```.
4. Build the project ```ci/bin/build.sh``` or ```c:\proj\libiuf> ci\bin\build.bat```.
4. Run the tests ```ci/bin/unittests.sh``` or ```c:\proj\libiuf> ci\bin\unittests.bat```.
5. Create a branch: git checkout -b foo-the-bars 1.3.

#### Making your Changes
1. Write tests expecting the correct/fixed functionality; make sure they fail.
2. Implement functionality to make the test pass.
3. Run tests again, making sure they pass.
4. Add changelog entry briefly describing your change.
5. Commit your changes: git commit -m "Foo the bars"

#### Creating Pull Requests
1. Push your commit to get it back up to your fork: git push origin HEAD
2. Visit Github, click handy “Pull request” button that it will make upon noticing your new branch.
3. In the description field, write down issue number (if submitting code fixing an existing issue) or describe the issue + your fix (if submitting a wholly new bugfix).
4. Hit ‘submit’! And please be patient - the maintainers will get to you when they can.

# How to report a bug

If you find a security vulnerability, do NOT open an issue. Contact us directly on [Slack](https://philips-software.slack.com/messages/CLAMQAKRS) or e-mail one of the [maintainers](MAINTAINERS.md) instead.

In order to determine whether you are dealing with a security issue, ask yourself these two questions:
* Can I access something that's not mine, or something I shouldn't have access to?
* Can I disable something for other people?

If the answer to either of those two questions are "yes", then you're probably dealing with a security issue. Note that even if you answer "no" to both questions, you may still be dealing with a security issue, so if you're unsure, just email us.

When filing an issue, make sure to answer these five questions:

1. What version compiler and compiler version are you using?
2. What operating system and processor architecture are you using?
3. What did you do?
4. What did you expect to see?
5. What did you see instead?