pipeline {
    agent none
    stage('Checkout') {
        git 'ssh://vcs-user@phabricator.dockstudios.co.uk/diffusion/SHAMEAN/shamean.git'
    }
    stage('Build') {
        agent {
            docker { image 'fare-docker-reg.dock.studios:5000/quay.io/eclipse/che-cpp-rhel7:nightly' }
        }
        steps {
            sh 'g++ -lcrypto -lssl -g main.cpp -o shamean'
        }
    }

    stage('Analyse') {
        sh 'sonar-scanner -Dsonar.projectKey=shamean -Dsonar.sources=. -Dsonar.host.url=http://sonarqube.dock.studios -Dsonar.login=106a9e583ada41e6d85dcd8f9a5177498203e4eb'
    }

    stage('Test') {
        sh 'echo hi'
    }

    stage('Archive') {
        sh "zip shamean-${env.BUILD_NUMBER}" shamean
        archiveArtifacts artifacts: 'shamean*.zip,shamean', fingerprint: true
    }
    stage('CleanUP') {
        cleanWs(patterns: [[pattern: 'shamean', type: 'INCLUDE']])
    }
}