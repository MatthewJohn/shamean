node {
    //stage('Checkout') {
    //    git branch: env.GIT_BRANCH, url: 'ssh://vcs-user@phabricator.dockstudios.co.uk/diffusion/SHAMEAN/shamean.git'
    //}
    stage('PullImage') {
        sh './scripts/build_container.sh --build-arg=http_proxy=http://fare-proxy.dock.studios:3142 --build-arg=https_proxy=http://fare-proxy.dock.studios:3142'
    }

    stage('Create version.h') {
        sh """cat <<EOF > version.hpp
#define SHAMEAN_VERSION "${env.BUILD_NUMBER}"
#define SHAMEAN_BUILD_DATE "`date`"
#define SHAMEAN_GIT_COMMIT "`git rev-parse HEAD`"
EOF
"""
    }

    docker.image('shamean-build').inside {
        stage('Build') {
            sh './scripts/build.sh'
        }

        stage('Test') {
            sh './scripts/run_tests.sh'
        }
    }

    stage('Analyse') {
        sh 'sonar-scanner -Dsonar.cfamily.cppunit.reportsPath=cppTestShameanResults.xml -Dsonar.projectKey=shamean -Dsonar.sources=. -Dsonar.host.url=http://sonarqube.dock.studios -Dsonar.login=106a9e583ada41e6d85dcd8f9a5177498203e4eb -Dsonar.cxx.includeDirectories=/usr/include,/usr/local/include,/usr/include/c++/5 -Dsonar.cfamily.gcov.reportsPath=. -Dsonar.cxx.coverage.reportPath=./*.xml -Dsonar.branch.name=$(git branch | head | sed \'s/^* //g\')'
    }

    stage('Archive') {
        sh "zip shamean-${env.BUILD_NUMBER}.zip shamean"
        archiveArtifacts artifacts: 'shamean*.zip,shamean', fingerprint: true
    }
    stage('CleanUP') {
        step([$class: 'WsCleanup'])
    }
}
