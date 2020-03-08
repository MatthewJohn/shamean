node {
    stage('Checkout') {
        git 'ssh://vcs-user@phabricator.dockstudios.co.uk/diffusion/SHAMEAN/shamean.git'
    }
    stage('PullImage') {
        sh 'docker build . -t shamean-build -f Dockerfile.build --build-arg=http_proxy=http://fare-proxy.dock.studios:3142 --build-arg=https_proxy=http://fare-proxy.dock.studios:3142'
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
            sh 'g++ -g main.cpp shamean.cpp -o shamean -lcrypto -static'
        }

        stage('Test') {
            sh 'g++ -g -I./ -o testshamean TestShamean.cpp shamean.o -lcppunit -lcrypto'
            sh './testshamean'
        }
    }

    stage('Analyse') {
        sh 'sonar-scanner -Dsonar.cfamily.cppunit.reportsPath=cppTestShameanResults.xml -Dsonar.projectKey=shamean -Dsonar.sources=. -Dsonar.host.url=http://sonarqube.dock.studios -Dsonar.login=106a9e583ada41e6d85dcd8f9a5177498203e4eb'
    }

    stage('Test') {
        sh 'echo hi'
    }

    stage('Archive') {
        sh "zip shamean-${env.BUILD_NUMBER}.zip shamean"
        archiveArtifacts artifacts: 'shamean*.zip,shamean', fingerprint: true
    }
    stage('CleanUP') {
        step([$class: 'WsCleanup'])
    }
}
