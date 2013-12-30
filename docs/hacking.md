CruceGame Community Guide (a.k.a. "HACKING")
=========

Use ideas from [Apache Subversion Community Guide (aka "HACKING")](http://subversion.apache.org/docs/community-guide/)
to document as many aspects of the development as possible.

Must-Haves:

* semantic versioning: http://semver.org/
  * use github to assign tasks to milestones. There will be five milestones
    during the initial development (until 1.0.0: 0.2.0, 0.4.0, 0.6.0 and 0.8.0)
* a branching model, for example http://nvie.com/posts/a-successful-git-branching-model/
  but there are other ones inspired by this just as successful. What matters most
  is to stick to it, once you've decided how you want to proceed
* test-driven development or at least testing after development - but each
  milestone must have complete tests which pass
  * code coverage should be over 50%
* the communication must be carried out in English (the source code, the documents,
  the mailing list if there will be one)
* a coding standard - you can choose which one and how fine grained you want it
  to be, but you must stick to it once you have chosen one


Working with Git
======

If you want to contribute to our project, you need to create a fork.
After you commit your changes, create a pull request. Your commits will be reviewed and integrated in project.

Branching model
------

There are 2 main branches with infinite lifetime: master and develop.
We consider origin/master to be the main branch where the source code of HEAD always reflects a production-ready state.
We consider origin/develop to be the main branch where the source code of HEAD always reflects a state with the latest delivered development changes for the next release.

So, every time you make a patch, it will be integrated in develop branch. 
At every release, the master is merged with develop.

###Feature branches

No one should make commits directly to develop branch. Instead, when you create a patch, you should create a feature branch.
All feature branches MUST be merged into develop. Also, they should only exist in developers' forks, not in origin.
They can hava any name, excep master, develop, release-*, hotfix-*.

When starting work on a new feature, branch of from develop branch:

'''$git checkout -b myfeature develop'''

To integrate a feature branch into develop:

'''$ git checkout develop
Switched to branch 'develop'
$ git merge --no-ff myfeature
Updating ea1b82a..05e9557
(Summary of changes)
$ git branch -d myfeature
Deleted branch myfeature (was 05e9557).
$ git push origin develop'''

The --no-ff flag causes the merge to always create a new commit object, even if the merge could be performed with a fast-forward.
This avoids losing information about the historical existence of a feature branch and groups together all commits that together added the feature. 

###Release branches

Release branches support preparation for a new production release. Their purpose is to allow small changes (correcting typos
small bug fixes), while develop branch remins open for new patches. All features tagged for the current version must
be integrated into develop by this time. Patches added to develop branch after this one was created will be added to the
next release. Release branches should be merged with master and develop.

Their name should be release-* and should indicate the version number.

###Hotfix branches

Hotfix branches are used to fix bugs in a production version. When a bug is found in a production version, a hotfix branch
should be branched off from master. After fixing the bug, it should be merged into master and develop. When a release branch
currently exists, hotfix branches must be merged into that instead of develop.

Their name should be hotfix-* and should indicate the version number.


The creation and merge of release and hotfix branches is simillar to those from feature branches.


Communication
======

 * Our main communication channel is the mailing list (abbreviated ML).
The ML address is cruce-development@googlegroups.com. This group is public (anyone can see the discussions and can send e-mails).
If you want to join the group, please write an e-mail and tell us your intentions.

The client or users may contact the development team using the mailing list.

 * Another communication channel used is the IRC channel #cruce-devel on the Freenode network.
Its primary purpose is instant communication between developers. Note that discussions on this channel are not official.
The decisions made on this channel *MUST* be consemnated on the mailing list. This channel is open.

 * There is also this group on Google+ available at the following URL: https://plus.google.com/u/0/communities/105057231196596797294/.
Although the group is private anyone is free to send a join request. Its primary purpose is to inform the client and the users about
the development of the application.

Authors
=======

Please document your names and your e-mail addresses.

* Gafton Paul (<paul.gafton@gmail.com>)

* Radu Szasz (<radu.szasz@gmail.com>)

* MarianG (<gherca.marian@gmail.com>)

* Sarvsav Sharma (<sarvsav@gmail.com>)


Contributors
=======

Here be committers who haven't contributed considerably.


Tutors/Coordinators
======

* Flavius Aspra (<flavius.as@gmail.com>)
* Dani J (<dani.user@gmail.com>)
