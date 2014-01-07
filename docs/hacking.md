CruceGame Community Guide (a.k.a. "HACKING")
=========

*Use ideas from [Apache Subversion Community Guide (aka "HACKING")]
(http://subversion.apache.org/docs/community-guide/) to document as many aspects
of the development as possible.*

*Must-Haves:*

* *semantic versioning: http://semver.org/*
  * *use github to assign tasks to milestones. There will be five milestones
    during the initial development (until 1.0.0: 0.2.0, 0.4.0, 0.6.0 and 0.8.0)*
* *a branching model, for example http://nvie.com/posts/a-successful-git-branching-model/
  but there are other ones inspired by this just as successful. What matters
  most is to stick to it, once you've decided how you want to proceed*
* *test-driven development or at least testing after development - but each
  milestone must have complete tests which pass*
  * *code coverage should be over 50%*
* *the communication must be carried out in English (the source code, the 
  documents, the mailing list if there will be one)*
* *a coding standard - you can choose which one and how fine grained you want it
  to be, but you must stick to it once you have chosen one*


Working with Git
======

If you want to contribute to our project, you need to create a fork.
After you commit your changes, create a pull request. Your commits will be
reviewed and integrated in project.

The commits
------
Do commit often small changes. The commits should be modular, every commit
representing one modification. The commit messages should be as short as
possible, yet useful. They should tell the story of the developing process
of the project. Commit names should start with a capital letter and use past
tense. A commit description is not needed.  
If your commit fixes an issue, include issue number in commit (e.g #14). This
will link your commit to the issue.

Being up-to-date
------
Add other forks and the main repository as remotes:

```git remote add remote_name remote_adress```

In our case, the main repository should be added as remote as follows:

```git remote add upstream https://github.com/danij/CruceGame.git```

Note that this is using https. You may use ssh as well.

Use ```git remote -v``` to view your remotes.

Use ```git fetch --all``` often to find out what other members of the team
have done in the meanwile.

Branching model
------

There are 2 main branches with infinite lifetime: master and develop.
* We consider origin/master to be the main branch where the source code of HEAD
  always reflects a production-ready state.
* We consider origin/develop to be the main branch where the source code of HEAD
  always reflects a state with the latest delivered development changes for the
  next release.

So, every time you make a patch, it will be integrated in develop branch. 
At every release, the master is merged with develop.

###Feature branches

No one should make commits directly to develop branch. Instead, when you create
a patch, you should create a feature branch.
All feature branches MUST be merged into develop. Also, they should only exist
in developers' forks, not in origin.
They can have any name, except master, develop, release-\*, hotfix-\*.

When starting work on a new feature, branch out of from develop branch:

```$git checkout -b myfeature develop```

To integrate a feature branch into develop:

```$ git checkout develop
Switched to branch 'develop'
$ git merge --no-ff myfeature
Updating ea1b82a..05e9557
(Summary of changes)
$ git branch -d myfeature
Deleted branch myfeature (was 05e9557).
$ git push origin develop```

The --no-ff flag causes the merge to always create a new commit object, even if
the merge could be performed with a fast-forward.
This avoids losing information about the historical existence of a feature
branch and groups together all commits that together added the feature. 

###Release branches

Release branches support preparation for a new production release. Their purpose
is to allow small changes (correcting typos small bug fixes), while develop
branch remins open for new patches. All features tagged for the current version
must be integrated into develop by this time. Patches added to develop branch
after this one was created will be added to the next release. Release branches
should be merged with master and develop.

Their name should be release-\* and should indicate the version number.

###Hotfix branches

Hotfix branches are used to fix bugs in a production version. When a bug is
found in a production version, a hotfix branch should be branched off from
master. After fixing the bug, it should be merged into master and develop. When
a release branch currently exists, hotfix branches must be merged into that
instead of develop.

Their name should be hotfix-\* and should indicate the version number.


The creation and merge of release and hotfix branches is simillar to those from
feature branches.

For more details about a simillar branching model, read the following resource:
http://nvie.com/posts/a-successful-git-branching-model/.

Issues
-----

When working on an issue, you will make another branch. Please name it
accordingly (*-bugfix for issues maked as bugs, no special name for features
and so on). It is not necessary to make one branch for each issue (some may
require more branches, or more related issues may be fixed in the same branch).
The commit names in that branch will contain in their message #xx, where xx is
the issue number (for example, #58), so git will automatically link the commit
to the issue. After your work has been reviewed, you will have to merge your
branch to the develop branch. When mergeing with no fast forward (as described
above), you will be asked for a commit message (as a merge commit will be
created). Please include after the default name 'Fix #xx', without quotes.
For example: Fix #58. Add this on a separate line. Make sure you specified
all fixed issues. For example:

```Merge branch 'doc-issues' into develop```
```Fix #30```
```Fix #48```

Do not use any other ways to close an issue that require code. Do not comment
things like "I have fixed the issue". If you use the commit names as I
described above, they will be automatically liked to the issue and we will see
them.


Communication
======

 * Our main communication channel is the mailing list (abbreviated ML).
The ML address is cruce-development@googlegroups.com. This group is public 
(anyone can see the discussions and can send e-mails).
If you want to join the group, please write an e-mail and tell us your
intentions.

The client or users may contact the development team using the mailing list.

Every discussion on mailing list should have a tag, placed in brackets at the
beginning of the subject.
For example ```[Doc] - Document communication channels``` is a possible subject
for a discussion.
The available tags are:  
\- ```Feature``` - discuss about developing a feature  
\- ```Doc``` - discuss about documentation  
\- ```Release``` - discuss about a release  
\- ```Programming``` - ask a programming question  
\- ```Meta``` - discussions about project that can not be fitted in one of above
                categories  
\- ```Talk``` - any other discussions, not related to the project  
\- ```Sign-up``` - send a sign in request

The mailing list automatically adds a discussion id to the subject. For example:
```[Cruce-devel: 233] [Feature] - This is a new feature```. When referring
to this discussion, please mention it's id, 233.

Also, note that the mailing list is moderated. If you make a mistake (i.e. a
wrong post), you will be asked to repost your message following our conventions
and your old message will be deleted. This shall be used only for flagrant
mistakes (for example, a post in another language than English).

 * Another communication channel used is the IRC channel #cruce-devel on the
   Freenode network.
   Its primary purpose is instant communication between developers. Note that
   discussions on this channel are not official.
   The decisions made on this channel *MUST* be consemnated on the mailing list.
   This channel is open.

 * There is also this group on Google+ available at the following URL:
 https://plus.google.com/u/0/communities/105057231196596797294/.
 Although the group is private anyone is free to send a join request.
 Its primary purpose is to inform the client and the users about the development
 of the application.


Testing
=======

For testing we use the cutter framework. It is recommended you read and
follow their tutorial: http://cutter.sourceforge.net/reference/tutorial.html,
as it covers both the use of cutter framework and build tools. Our build
scrips are very similar to those.
In cutter's reference: http://cutter.sourceforge.net/reference/ you will find
all the informations you need about installing and using the framework.
All functions MUST be tested. There should be tests for non-valid parameters
too (like NULLs for pointers).


Versioning
=======

For versioning we use semantic versioning: http://www.semver.org. The most
important things you need to know and modifications to that reference are:

1. Once a version has been released, its contents must not modified.
2. Version 0.x.y is for initial development.
3. The patch version must be reset to 0 when a new minor version is released.
Simillar, the minor and patch versions should be reset to 0 when a major version
is released.
4. This project will have 5 milestones: 0.2.0, 0.4.0, 0.6.0, 0.8.0 and 1.0.0.
The version 1.0.0 should respect all requirements described in
docs/requirements.md and all features described in docs/features.md.
5. Before releasing a version, make sure it is functional and passes all tests.
For testing, use tools described in docs/features.md. Do not release versions
with known bugs or with undocumented parts.



Authors
=======

Please document your names and your e-mail addresses.

* Gafton Paul (<paul.gafton@gmail.com>)

* Radu Szasz (<radu.szasz@gmail.com>)

* MarianG (<gherca.marian@gmail.com>)

* Sarvsav Sharma (<sarvsav@gmail.com>)

* Ionut Grt. (<ionut.grt@gmail.com>)

* Marius Leustean	(<marius.leus@gmail.com>)

* Vladu Emilian Sorin (<vladuemilian@gmail.com>)


Contributors
=======

Here be committers who haven't contributed considerably.


Tutors/Coordinators
======

* Flavius Aspra (<flavius.as@gmail.com>)
* Dani J (<dani.user@gmail.com>)
